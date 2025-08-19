#ifndef SAMPO_POI_H
#define SAMPO_POI_H

typedef enum { STORE, LANDMARK } PoiType;

typedef struct {
    PoiType type;
    char name[255];
} Poi;

typedef Poi *PoiPtr;

typedef PoiPtr *PoiList;

PoiPtr init_poi(const PoiType type, char *name);
void free_poi_list(PoiList poi_list, const int poi_counter);

#endif // SAMPO_POI_H
