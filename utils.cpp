#include "utils.hpp"
#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include<conio.h>
#include <atlsecurity.h> 
#include <string>
#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
std::string GetDiskSerialNumber()
{
    std::string result;
    HANDLE hDevice = INVALID_HANDLE_VALUE;

    hDevice = CreateFile(
        "\\\\.\\PhysicalDrive0",           // Drive to open
        0,                                 // No access to the drive
        FILE_SHARE_READ | FILE_SHARE_WRITE,// Share mode
        NULL,                              // Default security attributes
        OPEN_EXISTING,                     // Open existing device
        0,                                 // No attributes
        NULL                               // No template file
    );

    if (hDevice == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error("Failed to open device handle");
    }

    STORAGE_PROPERTY_QUERY storagePropertyQuery{};
    DWORD dwBytesReturned = 0;
    char buffer[512]{};

    storagePropertyQuery.PropertyId = StorageDeviceProperty;
    storagePropertyQuery.QueryType = PropertyStandardQuery;

    if (!DeviceIoControl(
        hDevice,                           // Device handle
        IOCTL_STORAGE_QUERY_PROPERTY,     // IOCTL to send
        &storagePropertyQuery,             // Input buffer
        sizeof(storagePropertyQuery),      // Input buffer size
        &buffer,                           // Output buffer
        sizeof(buffer),                    // Output buffer size
        &dwBytesReturned,                  // Number of bytes returned
        NULL))                             // Overlapped
    {
        CloseHandle(hDevice);
        throw std::runtime_error("Failed to send IOCTL_STORAGE_QUERY_PROPERTY");
    }

    STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(&buffer);
    DWORD dwSerialNumberOffset = pDeviceDescriptor->SerialNumberOffset;
    if (dwSerialNumberOffset != 0)
    {
        result = &buffer[dwSerialNumberOffset];
    }

    CloseHandle(hDevice);

    return result;
}
std::string utils::get_hwid() {
	ATL::CAccessToken accessToken;
	ATL::CSid currentUserSid;
	std::string Disk = GetDiskSerialNumber();
	if (!Disk.empty())
		return Disk;
	return "none";
}

std::time_t utils::string_to_timet(std::string timestamp) {
	auto cv = strtol(timestamp.c_str(), NULL, 10);

	return (time_t)cv;
}

std::tm utils::timet_to_tm(time_t timestamp) {
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}
