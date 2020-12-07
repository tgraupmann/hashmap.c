#include "../hashmap.h"

#include <pthread.h>
#include <stdio.h>
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
	
	if (map != NULL)
	{

		//workaround issue with deleted items showing up
        /*
		struct MapStringId search;
		search._mKey = map->_mKey;
		search._mId = -1;
		struct MapStringId* kvp = hashmap_get(_gMapID, &search);
		if (kvp == NULL)
		{
			fprintf(stdout, "Hash item already deleted.\r\n");
			return true;
		}
        */

    	printf("Hash Item Id=%d Name=%s\r\n", map->_mId, map->_mKey);
	}
    return true;
}

uint64_t MapStringId_Hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct MapStringId* map = item;
    return hashmap_sip(map->_mKey, strlen(map->_mKey), seed0, seed1);
}

#pragma endregion



void* UnitTest_Strings(void* arg)
{
    const int numKeys = 100;
    for (int i = 0; i < numKeys; ++i)
    {
        char computeName[256];
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
    }

    for (int i = 0; i < numKeys; ++i)
    {
        char computeName[256];
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
    }

    for (int i = numKeys - 1; i >= 0; --i)
    {
        char computeName[256];
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
    }

    pthread_exit(NULL);

    return NULL;
}

int main()
{
    _gMapID = hashmap_new(sizeof(struct MapStringId), 0, 0, 0, 			MapStringId_Hash, MapStringId_Compare, NULL);

    pthread_t threadTest;
	if (0 != pthread_create(&threadTest, NULL, &UnitTest_Strings, NULL))
	{
		fprintf(stderr, "Failed to create test thread!\r\n");
		return -1;
	}

    pthread_join(threadTest, NULL);

    fprintf(stdout, "Hashmap should be empty!\r\n");
    hashmap_scan(_gMapID, MapStringId_Iter, NULL);

    return 0;
}
