#include "poi.h"

#include <stdlib.h>
#include <string.h>

#include "logger.h"

PoiPtr init_poi(const PoiType type, char *name) {
    if (name == NULL) {
        fetal(&global_logger, "init_poi: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    PoiPtr poi = (PoiPtr)malloc(sizeof(Poi));
    if (poi == NULL) {
        fetal(&global_logger, "init_poi: POI Initialization Failed");
        exit(EXIT_FAILURE);
    }

    poi->type = type;
    strncpy(poi->name, name, sizeof(poi->name) - 1);
    poi->name[sizeof(poi->name) - 1] = '\0';

    return poi;
}

void free_poi_list(PoiList poi_list, const int poi_counter) {
    if (poi_list == NULL) {
        fetal(&global_logger, "free_poi_list: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < poi_counter; i++) {
        free(poi_list[i]);
        poi_list[i] = NULL;
    }
    free(poi_list);
    poi_list = NULL;
}
