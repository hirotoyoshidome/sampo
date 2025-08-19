#include <assert.h>
#include <stdlib.h>

#include "poi.h"

void test_poi_001() {
    PoiPtr poi = init_poi(STORE, "test");
    assert(poi != NULL);

    int poi_counter = 1;
    PoiList pois = (PoiList)malloc(sizeof(PoiPtr) * poi_counter);
    pois[0] = poi;

    free_poi_list(pois, poi_counter);
    assert(pois != NULL);
    pois = NULL;
    assert(pois == NULL);
}
