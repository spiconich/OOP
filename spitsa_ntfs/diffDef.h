#pragma once
#pragma pack(push,1)

typedef void (*checkPhys)(bool&, int, std::string);
typedef void (*checkVol)(bool&, bool&, bool&,std::string, std::string,std::string&);
typedef std::string(*tellMeFStype)(bool&, bool&, bool&, std::string);
typedef int (*tellMeFSTryResult)(std::string);
typedef void (*showMeAllInfo)(std::string);