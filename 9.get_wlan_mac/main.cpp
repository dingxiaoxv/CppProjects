#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>

std::string getMacAddress(const std::string &interface_name)
{
    struct ifreq ifr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        std::cerr << "Error opening socket." << std::endl;
        return "";
    }

    std::memset(&ifr, 0, sizeof(ifr));
    std::strcpy(ifr.ifr_name, interface_name.c_str());

    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1)
    {
        std::cerr << "Error getting MAC address." << std::endl;
        close(sockfd);
        return "";
    }

    close(sockfd);

    unsigned char mac_addr[6];
    std::memcpy(mac_addr, ifr.ifr_hwaddr.sa_data, 6);

    char mac_str[18];
    std::sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X",
                 mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

    return std::string(mac_str);
}

int main(int argc, char **argv)
{
    std::string interface_name(argv[1]);
    std::string mac_address = getMacAddress(interface_name);

    if (!mac_address.empty())
    {
        std::cout << "MAC Address of " << interface_name << ": " << mac_address << std::endl;
    }
    else
    {
        std::cerr << "Failed to retrieve MAC address of " << interface_name << "." << std::endl;
    }

    return 0;
}
