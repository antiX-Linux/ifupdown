#include <stddef.h>
#include "header.h"


#include "archcommon.h"
#include "archlinux.h"


static option_default _none_default[] = {
	{ NULL, NULL }
};
static conversion _none_conv[] = {
	{ NULL, NULL, NULL, 0, NULL }
};
static int _none_up(interface_defn *ifd, execfn *exec) {
return 1;
}
static int _none_down(interface_defn *ifd, execfn *exec) {
return 1;
}
static method methods[] = {
        {
                "none",
                _none_up, _none_down,
                _none_conv, _none_default
        },
};

address_family addr_meta = {
        "meta",
        sizeof(methods)/sizeof(struct method),
        methods
};
