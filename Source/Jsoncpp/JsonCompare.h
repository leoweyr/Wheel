#pragma once

#include "Jsoncpp.h"

namespace Json{
    Json::Value Compare(const Json::Value compared, const Json::Value referred, Json::Value &same);
}