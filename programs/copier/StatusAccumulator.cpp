#include "StatusAccumulator.h"

#include <Poco/JSON/Parser.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>

#include <iostream>

namespace DB
{

StatusAccumulator::MapPtr StatusAccumulator::fromJSON(String state_json)
{
    Poco::JSON::Parser parser;
    auto state = parser.parse(state_json).extract<Poco::JSON::Object::Ptr>();
    MapPtr result_ptr = std::make_shared<Map>();
    for (const auto & table_name : state->getNames())
    {
        auto table_status_json = state->getValue<String>(table_name);
        auto table_status = parser.parse(table_status_json).extract<Poco::JSON::Object::Ptr>();
        /// Map entry will be created if it is absent
        auto & map_table_status = (*result_ptr)[table_name];
        map_table_status.all_partitions_count += table_status->getValue<size_t>("all_partitions_count");
        map_table_status.processed_partitions_count += table_status->getValue<size_t>("processed_partitions_count");
    }
    return result_ptr;
}

String StatusAccumulator::serializeToJSON(MapPtr statuses)
{
    Poco::JSON::Object result_json;
    for (const auto & [table_name, table_status] : *statuses)
    {
        Poco::JSON::Object status_json;
        status_json.set("all_partitions_count", table_status.all_partitions_count);
        status_json.set("processed_partitions_count", table_status.processed_partitions_count);

        result_json.set(table_name, status_json);
    }
    std::ostringstream oss;     // STYLE_CHECK_ALLOW_STD_STRING_STREAM
    oss.exceptions(std::ios::failbit);
    Poco::JSON::Stringifier::stringify(result_json, oss);
    auto result = oss.str();
    return result;
}

}
