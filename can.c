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
  if (!execute("ip link set %iface% type can bitrate %bitrate%", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[ ip link set %iface% type can loopback %loopback% ]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[ ip link set %iface% type can listen-only %listenonly% ]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[ ip link set %iface% type can triple-sampling %triple% ]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[ ip link set %iface% type can one-shot %oneshot% ]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[ ip link set %iface% type can berr-reporting %berr% ]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("ip link set %iface% up", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _static_down(interface_defn *ifd, execfn *exec) {
{
  if (!execute("ip link set %iface% down", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static method methods[] = {
        {
                "static",
                _static_up, _static_down,
                _static_conv, _static_default
        },
};

address_family addr_can = {
        "can",
        sizeof(methods)/sizeof(struct method),
        methods
};
