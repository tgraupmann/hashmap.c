#include "../hashmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Hashmaps
struct hashmap* _gMapID = NULL;


// define mapping

#pragma region MapStringId

struct MapStringId {
    const char* _mKey;
	int _mId;
};

int MapStringId_Compare(const void *a, const void *b, void *udata) {
    const struct MapStringId *ua = a;
    const struct MapStringId *ub = b;
    return strcmp(ua->_mKey, ub->_mKey);
}

bool MapStringId_Iter(const void *item, void *udata) {
    const struct MapStringId* map = item;
	printf("Hash Item Id=%d Name=%s\r\n", map->_mId, map->_mKey);
    return true;
}

uint64_t MapStringId_Hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct MapStringId* map = item;
    return hashmap_sip(map->_mKey, strlen(map->_mKey), seed0, seed1);
}

#pragma endregion



void UnitTest_Strings()
{
    const int numKeys = 100;
    for (int i = 0; i < numKeys; ++i)
    {
        char* computeName = malloc(sizeof(char) * 255);
		strcpy(computeName, "SomePrefix");
		char numberName[10];
		sprintf(numberName, "%d", i);
		strcat(computeName, numberName);
		strcat(computeName, ".SomeExtension");
        fprintf(stdout, "Adding to hash Name=%s Id=%d\r\n", computeName, i);

        struct MapStringId map;
        map._mKey = computeName;
        map._mId = i;
        hashmap_set(_gMapID, &map);

        free(computeName);
    }

    for (int i = 0; i < numKeys; ++i)
    {
        char* computeName = malloc(sizeof(char) * 255);
		strcpy(computeName, "SomePrefix");
		char numberName[10];
		sprintf(numberName, "%d", i);
		strcat(computeName, numberName);
		strcat(computeName, ".SomeExtension");

        struct MapStringId search;
        search._mKey = computeName;
        search._mId = -1;
        struct MapStringId* kvp = hashmap_get(_gMapID, &search);

        fprintf(stdout, "Hash item exists Name=%s Id=%d\r\n", kvp->_mKey, kvp->_mId);

        free(computeName);
    }

    for (int i = numKeys - 1; i >= 0; --i)
    {
        char* computeName = malloc(sizeof(char) * 255);
		strcpy(computeName, "SomePrefix");
		char numberName[10];
		sprintf(numberName, "%d", i);
		strcat(computeName, numberName);
		strcat(computeName, ".SomeExtension");

        fprintf(stdout, "Removing from hash Name=%s Id=%d\r\n", computeName, i);

        struct MapStringId map;
        map._mKey = computeName;
        map._mId = -1;
        hashmap_delete(_gMapID, &map);

        free(computeName);
    }
}

int main()
{
    _gMapID = hashmap_new(sizeof(struct MapStringId), 0, 0, 0, 			MapStringId_Hash, MapStringId_Compare, NULL);

    UnitTest_Strings();

    fprintf(stdout, "Hashmap should be empty!\r\n");
    hashmap_scan(_gMapID, MapStringId_Iter, NULL);

    return 0;
}
