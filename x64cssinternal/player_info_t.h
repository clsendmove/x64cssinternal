#pragma once
#define MAX_PLAYER_NAME_LENGTH 32
#define SIGNED_GUID_LEN 32
#define MAX_CUSTOM_FILES 4

using CRC32_t = unsigned int;

struct player_info_t
{
	char name[MAX_PLAYER_NAME_LENGTH]{};
	int userID{};
	char guid[SIGNED_GUID_LEN + 1]{};
	unsigned int friendsID{};
	char friendsName[MAX_PLAYER_NAME_LENGTH]{};
	bool fakeplayer{};
	bool ishltv{};
	bool isreplay{};
	CRC32_t customFiles[MAX_CUSTOM_FILES]{};
	unsigned char filesDownloaded{};
};
