#include <stddef.h>
#include "header.h"


#include "archcommon.h"
#include "archlinux.h"


static option_default _none_default[] = {
	{ "description", "" },
	{ NULL, NULL }
};
static conversion _none_conv[] = {
	{ "iface", "link", get_token, 3, (char * []){".", "0", ""} },
	{ "iface", "vlan_id0", get_token, 3, (char * []){".", "1", ""} },
	{ "iface", "iface0", get_token, 3, (char * []){":", "0", ""} },
	{ "vlan_id0", "vlan_id1", get_token, 3, (char * []){":", "0", ""} },
	{ "vlan_id1", "vlan_id", to_decimal, 1, (char * []){"10"} },
	{ NULL, NULL, NULL, 0, NULL }
};
static int _none_up(interface_defn *ifd, execfn *exec) {
if ( iface_has(".") ) {
  if (!execute("if test -d /sys/class/net/%link% -a \
            ! -d /sys/class/net/%iface0% ; \
    then \
        if test -d /sys/class/net/%link%/device/infiniband; then \
            echo 0x%vlan_id1% > /sys/class/net/%link%/create_child; \
        else \
            /bin/ip link set up dev %link%; \
            /bin/ip link add link %link% name %iface0% type vlan id %vlan_id%; \
	fi; \
    fi", ifd, exec) && !ignore_failures) return 0;
}
if ( iface_is_lo() ) {
  if (!execute("-/bin/ip link set up dev %iface% 2>/dev/null", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[/bin/ip link set %iface0% alias \"%description%\"]]", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _none_down(interface_defn *ifd, execfn *exec) {
if ( iface_has(".") && !iface_has(":") ) {
  if (!execute("if test -d /sys/class/net/%link%/device/infiniband; then \
        echo 0x%vlan_id1% > /sys/class/net/%link%/delete_child; \
    else \
        /bin/ip link del %iface%; \
    fi", ifd, exec) && !ignore_failures) return 0;
}
if ( iface_is_lo() ) {
  if (!execute("-/bin/ip link set down dev %iface% 2>/dev/null", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[-test -n \"%description%\" && /bin/echo > /sys/class/net/%iface0%/ifalias]]", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static method methods[] = {
        {
                "none",
                _none_up, _none_down,
                _none_conv, _none_default
        },
};

address_family addr_link = {
        "link",
        sizeof(methods)/sizeof(struct method),
        methods
};
