#include <utils/CommonUtils.h>

Json::Value JsonUtils::ConvertMapToJson(std::map<std::string, std::string> map)
{
    Json::Value json;

    for (auto &element : map)
    {
        json[element.first] = element.second;
    }
    return json;
}

std::map<std::string, std::string> JsonUtils::ConvertJsonToMap(Json::Value json)
{
    std::map<std::string, std::string> map;

    for (auto element = json.begin(); element != json.end(); element++)
    {
        if (!element.key().empty())
        {
            map.insert({element.key().asString(), (*element).asString()});
        }
    }

    return map;
}

std::string StringUtil::Trim(const std::string &str, const std::string &delimit)
{
    auto begin = str.find_first_not_of(delimit);
    if (begin == std::string::npos)
    {
        return "";
    }
    auto end = str.find_last_not_of(delimit);
    return str.substr(begin, end - begin + 1);
}

std::string StringUtil::TrimLeft(const std::string &str, const std::string &delimit)
{
    auto begin = str.find_first_not_of(delimit);
    if (begin == std::string::npos)
    {
        return "";
    }
    return str.substr(begin);
}

std::string StringUtil::TrimRight(const std::string &str, const std::string &delimit)
{
    auto end = str.find_last_not_of(delimit);
    if (end == std::string::npos)
    {
        return "";
    }
    return str.substr(0, end);
}

std::string StringUtil::WStringToString(const std::wstring &ws)
{
    std::string str_locale = setlocale(LC_ALL, "");
    const wchar_t *wch_src = ws.c_str();
    size_t n_dest_size = wcstombs(NULL, wch_src, 0) + 1;
    char *ch_dest = new char[n_dest_size];
    memset(ch_dest, 0, n_dest_size);
    wcstombs(ch_dest, wch_src, n_dest_size);
    std::string str_result = ch_dest;
    delete[] ch_dest;
    setlocale(LC_ALL, str_locale.c_str());
    return str_result;
}

std::wstring StringUtil::StringToWString(const std::string &s)
{
    std::string str_locale = setlocale(LC_ALL, "");
    const char *chSrc = s.c_str();
    size_t n_dest_size = mbstowcs(NULL, chSrc, 0) + 1;
    wchar_t *wch_dest = new wchar_t[n_dest_size];
    wmemset(wch_dest, 0, n_dest_size);
    mbstowcs(wch_dest, chSrc, n_dest_size);
    std::wstring wstr_result = wch_dest;
    delete[] wch_dest;
    setlocale(LC_ALL, str_locale.c_str());
    return wstr_result;
}

std::vector<std::string> StringUtil::mySplit(const std::string &str, std::string sp_string) // split(), str 是要分割的string
{
    std::vector<std::string> vecString;
    int sp_stringLen = sp_string.size();
    int lastPosition = 0;
    int index = -1;
    //如果要分割的串就是自己怎么写？
    while (-1 != (index = str.find(sp_string, lastPosition)))
    {
        if (str.substr(lastPosition, index - lastPosition) != std::string("")) //避免第一个字符串就是sp_string
        {
            vecString.push_back(str.substr(lastPosition, index - lastPosition));
        }
        lastPosition = index + sp_stringLen;
    }
    std::string lastStr = str.substr(lastPosition);
    if (!lastStr.empty())
    {
        vecString.push_back(lastStr);
    }
    return vecString;
}

void StringUtil::parseParam(std::string paramsStr, std::map<std::string, std::string> &reqParams)
{

    std::vector<std::string> pairslist = StringUtil::mySplit(paramsStr, "&");

    for (size_t i = 0; i < pairslist.size(); ++i)
    {
        std::string name = pairslist[i].substr(0, pairslist[i].find("="));
        std::string value = pairslist[i].substr(pairslist[i].find("=") + 1);

        reqParams[name] = value;
    }
}

void StringUtil::ClearHeadTailSpace(std::string &str)
{
    if (str.empty())
    {
        return;
    }

    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
}

std::string StringUtil::getFileSuffix(std::string str)
{
    if (str.empty())
    {
        return str;
    }

    int pos = str.find_last_of('.');

    if (pos == -1)
    {
        return "null";
    }
    else
    {
        return str.substr(pos + 1);
    }
}
