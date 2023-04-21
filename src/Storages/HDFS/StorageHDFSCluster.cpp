#include "config.h"
#include "Interpreters/Context_fwd.h"

#if USE_HDFS

#include <Storages/HDFS/StorageHDFSCluster.h>

#include <Core/QueryProcessingStage.h>
#include <DataTypes/DataTypeString.h>
#include <Interpreters/getHeaderForProcessingStage.h>
#include <Interpreters/InterpreterSelectQuery.h>
#include <QueryPipeline/RemoteQueryExecutor.h>

#include <Processors/Transforms/AddingDefaultsTransform.h>

#include <Processors/Sources/RemoteSource.h>
#include <Parsers/ASTTablesInSelectQuery.h>

#include <Storages/IStorage.h>
#include <Storages/SelectQueryInfo.h>
#include <Storages/HDFS/HDFSCommon.h>

#include <memory>


namespace DB
{

StorageHDFSCluster::StorageHDFSCluster(
    ContextPtr context_,
    String cluster_name_,
    const String & uri_,
    const StorageID & table_id_,
    const String & format_name_,
    const ColumnsDescription & columns_,
    const ConstraintsDescription & constraints_,
    const String & compression_method_,
    size_t table_function_max_arguments,
    bool structure_argument_was_provided_)
    : IStorageCluster(cluster_name_, table_id_, &Poco::Logger::get("StorageHDFSCluster (" + table_id_.table_name + ")"), table_function_max_arguments, structure_argument_was_provided_)
    , uri(uri_)
    , format_name(format_name_)
    , compression_method(compression_method_)
{
    checkHDFSURL(uri_);
    context_->getRemoteHostFilter().checkURL(Poco::URI(uri_));

    StorageInMemoryMetadata storage_metadata;

    if (columns_.empty())
    {
        auto columns = StorageHDFS::getTableStructureFromData(format_name, uri_, compression_method, context_);
        storage_metadata.setColumns(columns);
    }
    else
        storage_metadata.setColumns(columns_);

    storage_metadata.setConstraints(constraints_);
    setInMemoryMetadata(storage_metadata);
}

RemoteQueryExecutor::Extension StorageHDFSCluster::getTaskIteratorExtension(ASTPtr, ContextPtr context) const
{
    auto iterator = std::make_shared<HDFSSource::DisclosedGlobIterator>(context, uri);
    auto callback = std::make_shared<HDFSSource::IteratorWrapper>([iter = std::move(iterator)]() mutable -> String { return iter->next(); });
    return RemoteQueryExecutor::Extension{.task_iterator = std::move(callback)};
}

NamesAndTypesList StorageHDFSCluster::getVirtuals() const
{
    return NamesAndTypesList{
        {"_path", std::make_shared<DataTypeLowCardinality>(std::make_shared<DataTypeString>())},
        {"_file", std::make_shared<DataTypeLowCardinality>(std::make_shared<DataTypeString>())}};
}

}

#endif
