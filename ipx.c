#include <stddef.h>
#include "header.h"


#include "archcommon.h"
#include "archlinux.h"


static option_default _static_default[] = {
	{ NULL, NULL }
};
static conversion _static_conv[] = {
	{ NULL, NULL, NULL, 0, NULL }
};
static int _static_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("ipx_interface add %iface% %frame% %netnum%", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _static_down(interface_defn *ifd, execfn *exec) {
{
  if (!execute("ipx_interface del %iface% %frame%", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _dynamic_default[] = {
	{ NULL, NULL }
};
static conversion _dynamic_conv[] = {
	{ NULL, NULL, NULL, 0, NULL }
};
static int _dynamic_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("ipx_interface add %iface% %frame%", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _dynamic_down(interface_defn *ifd, execfn *exec) {
{
  if (!execute("ipx_interface del %iface% %frame%", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static method methods[] = {
        {
                "dynamic",
                _dynamic_up, _dynamic_down,
                _dynamic_conv, _dynamic_default
        },
        {
                "static",
                _static_up, _static_down,
                _static_conv, _static_default
        },
};

address_family addr_ipx = {
        "ipx",
        sizeof(methods)/sizeof(struct method),
        methods
};
