#include <fstream>
#include <set>

#include "config.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void Config::load(const char *path)
{
    std::ifstream config_fstream(path);
    json j;
    config_fstream >> j;

    client_id = j.at("oauth").at("client").at("id").get<std::string>();
    client_secret = j.at("oauth").at("client").at("secret").get<std::string>();
    scope = j.at("oauth").at("scope").get<std::string>();
    device_endpoint = j.at("oauth").at("device_endpoint").get<std::string>();
    token_endpoint = j.at("oauth").at("token_endpoint").get<std::string>();
    userinfo_endpoint = j.at("oauth").at("userinfo_endpoint").get<std::string>();
    username_attribute = j.at("oauth").at("username_attribute").get<std::string>();
    qr_error_correction_level = j.at("qr").at("error_correction_level").get<int>();
    if (j.find("remote_auth") != j.end())
    {
        remote_auth_url = j.at("remote_auth").at("url").get<std::string>();
        remote_auth_id = j.at("remote_auth").at("id").get<std::string>();
        remote_auth_secret = j.at("remote_auth").at("secret").get<std::string>();
        remote_auth_response_ok = j.at("remote_auth").at("response_ok").get<std::string>();
    }
    if (j.find("ldap") != j.end())
    {
        ldap_host = j.at("ldap").at("host").get<std::string>();
        ldap_basedn = j.at("ldap").at("basedn").get<std::string>();
        ldap_user = j.at("ldap").at("user").get<std::string>();
        ldap_passwd = j.at("ldap").at("passwd").get<std::string>();
        ldap_filter = j.at("ldap").at("filter").get<std::string>();
        ldap_attr = j.at("ldap").at("attr").get<std::string>();
    }
    if (j.find("users") != j.end())
    {
        for (auto &element : j["users"].items())
        {
            for (auto &local_user : element.value())
            {
                if (usermap.find(element.key()) == usermap.end())
                {
                    std::set<std::string> userset;
                    userset.insert((std::string)local_user);
                    usermap[element.key()] = userset;
                }
                else
                {
                    usermap[element.key()].insert((std::string)local_user);
                }
            }
        }
    }
}