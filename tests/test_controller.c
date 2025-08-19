#include <assert.h>

#include "controller.h"
#include "route.h"

void test_controller_001() {
    Controller *c = init_controller();
    assert(c != NULL);
    assert(c->route_counter == 0);

    RoutePtr route = init_route();
    add_route(c, route);
    assert(c->route_counter == 1);
    assert(c->routes[0] != NULL);

    free_controller(c);
    // assert(c->routes == NULL);

    assert(c != NULL);
    c = NULL;
    assert(c == NULL);
}
