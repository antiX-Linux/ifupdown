#include <stddef.h>
#include "header.h"


#include "archcommon.h"
#include "archlinux.h"


static option_default _loopback_default[] = {
	{ NULL, NULL }
};
static conversion _loopback_conv[] = {
	{ NULL, NULL, NULL, 0, NULL }
};
static int _loopback_up(interface_defn *ifd, execfn *exec) {
if ( !iface_is_lo() ) {
  if (!execute("/bin/ip link set dev %iface% up", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _loopback_down(interface_defn *ifd, execfn *exec) {
if ( !iface_is_lo() ) {
  if (!execute("/bin/ip link set dev %iface% down", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _static_default[] = {
	{ "broadcast", "+" },
	{ NULL, NULL }
};
static conversion _static_conv[] = {
	{ "hwaddress", NULL, cleanup_hwaddress, 0, NULL },
	{ "address", "netmask?", compute_v4_mask, 0, NULL },
	{ "address", NULL, compute_v4_addr, 0, NULL },
	{ "broadcast", NULL, compute_v4_broadcast, 0, NULL },
	{ NULL, NULL, NULL, 0, NULL }
};
static int _static_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/bin/ip addr add %address%[[/%netmask%]] [[broadcast %broadcast%]] \
	[[peer %pointopoint%]] [[scope %scope%]] dev %iface% label %iface%", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip link set dev %iface% [[mtu %mtu%]] [[address %hwaddress%]] up", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[ /bin/ip route add default via %gateway% [[metric %metric%]] dev %iface% onlink ]]", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _static_down(interface_defn *ifd, execfn *exec) {
{
  if (!execute("[[ /bin/ip route del default via %gateway% [[metric %metric%]] dev %iface% 2>&1 1>/dev/null || true ]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip addr del %address%[[/%netmask%]] [[broadcast %broadcast%]] \
	[[peer %pointopoint%]] [[scope %scope%]] dev %iface% label %iface%", ifd, exec) && !ignore_failures) return 0;
}
if ( iface_is_link() ) {
  if (!execute("/bin/ip link set dev %iface% down", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _manual_default[] = {
	{ NULL, NULL }
};
static conversion _manual_conv[] = {
	{ "hwaddress", NULL, cleanup_hwaddress, 0, NULL },
	{ NULL, NULL, NULL, 0, NULL }
};
static int _manual_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("[[/bin/ip link set dev %iface% mtu %mtu%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[/bin/ip link set dev %iface% address %hwaddress%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip link set dev %iface% up 2>/dev/null || true", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _manual_down(interface_defn *ifd, execfn *exec) {
if ( iface_is_link() && !do_all ) {
  if (!execute("/bin/ip link set dev %iface% down 2>/dev/null || true", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _dhcp_default[] = {
	{ NULL, NULL }
};
static conversion _dhcp_conv[] = {
	{ "hwaddress", NULL, cleanup_hwaddress, 0, NULL },
	{ NULL, NULL, NULL, 0, NULL }
};
static int _dhcp_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("[[/bin/ip link set dev %iface% address %hwaddress%]]", ifd, exec) && !ignore_failures) return 0;
}
if ( execable("/sbin/dhclient") ) {
  if (!execute("/sbin/dhclient -4 -v -pf /run/dhclient.%iface%.pid -lf /var/lib/dhcp/dhclient.%iface%.leases -I -df /var/lib/dhcp/dhclient6.%iface%.leases %iface% \
	[[-e IF_METRIC=%metric%]]", ifd, exec) && !ignore_failures) return 0;
}
else if ( execable("/sbin/pump") ) {
  if (!execute("/sbin/pump -i %iface% [[-h %hostname%]] [[-l %leasehours%]]", ifd, exec) && !ignore_failures) return 0;
}
else if ( execable("/sbin/udhcpc") ) {
  if (!execute("/sbin/udhcpc -n -p /run/udhcpc.%iface%.pid -i %iface% [[-x hostname:%hostname%]]", ifd, exec) && !ignore_failures) return 0;
}
else if ( execable("/sbin/dhcpcd") ) {
  if (!execute("/sbin/dhcpcd [[-h %hostname%]] [[-i %vendor%]] [[-I %client%]] \
           [[-l %leasetime%]] [[-m %metric%]] %iface%", ifd, exec) && !ignore_failures) return 0;
}
else if ( 1 ) {
  if (!execute("echo 'No DHCP client software found!' >/dev/stderr; false", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _dhcp_down(interface_defn *ifd, execfn *exec) {
if ( execable("/sbin/dhclient") ) {
  if (!execute("/sbin/dhclient -4 -v -r -pf /run/dhclient.%iface%.pid -lf /var/lib/dhcp/dhclient.%iface%.leases -I -df /var/lib/dhcp/dhclient6.%iface%.leases %iface%", ifd, exec) && !ignore_failures) return 0;
}
else if ( execable("/sbin/pump") ) {
  if (!execute("/sbin/pump -i %iface% -r", ifd, exec) && !ignore_failures) return 0;
}
else if ( execable("/sbin/udhcpc") ) {
  if (!execute("if test -f /run/udhcpc.%iface%.pid; then kill -USR2 $(/bin/cat /run/udhcpc.%iface%.pid); kill -TERM $(/bin/cat /run/udhcpc.%iface%.pid); fi", ifd, exec) && !ignore_failures) return 0;
}
else if ( execable("/sbin/dhcpcd") ) {
  if (!execute("/sbin/dhcpcd -k %iface%", ifd, exec) && !ignore_failures) return 0;
}
else if ( 1 ) {
  if (!execute("echo 'No DHCP client software found!' >/dev/stderr; false", ifd, exec) && !ignore_failures) return 0;
}
if ( iface_is_link() ) {
  if (!execute("/bin/ip link set dev %iface% down", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _bootp_default[] = {
	{ NULL, NULL }
};
static conversion _bootp_conv[] = {
	{ NULL, NULL, NULL, 0, NULL }
};
static int _bootp_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/sbin/bootpc [[--bootfile %bootfile%]] --dev %iface% [[--server %server%]] \
           [[--hwaddr %hwaddr%]] --returniffail --serverbcast", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _bootp_down(interface_defn *ifd, execfn *exec) {
if ( iface_is_link() ) {
  if (!execute("/bin/ip link set dev %iface% down", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _tunnel_default[] = {
	{ NULL, NULL }
};
static conversion _tunnel_conv[] = {
	{ NULL, NULL, NULL, 0, NULL }
};
static int _tunnel_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/bin/ip tunnel add %iface% mode %mode% remote %endpoint% [[local %local%]] \
       [[ttl %ttl%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip link set %iface% up [[mtu %mtu%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip addr add %address%/%netmask% dev %iface% [[peer %dstaddr%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[ /bin/ip route add default via %gateway% [[metric %metric%]] dev %iface% onlink ]]", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _tunnel_down(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/bin/ip tunnel del %iface%", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _ppp_default[] = {
	{ NULL, NULL }
};
static conversion _ppp_conv[] = {
	{ NULL, NULL, NULL, 0, NULL }
};
static int _ppp_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/usr/bin/pon [[%provider%]] [[unit %unit%]] [[%options%]]", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _ppp_down(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/usr/bin/poff [[%provider%]]", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _wvdial_default[] = {
	{ NULL, NULL }
};
static conversion _wvdial_conv[] = {
	{ NULL, NULL, NULL, 0, NULL }
};
static int _wvdial_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/sbin/start-stop-daemon --start -x /usr/bin/wvdial \
                      -p /run/wvdial.%iface%.pid -b -m -- [[ %provider% ]]", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _wvdial_down(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/sbin/start-stop-daemon --stop -x /usr/bin/wvdial \
                      -p /run/wvdial.%iface%.pid -s 2", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _ipv4ll_default[] = {
	{ NULL, NULL }
};
static conversion _ipv4ll_conv[] = {
	{ NULL, NULL, NULL, 0, NULL }
};
static int _ipv4ll_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/usr/sbin/avahi-autoipd -D %iface%", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _ipv4ll_down(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/usr/sbin/avahi-autoipd --kill %iface%", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static method methods[] = {
        {
                "bootp",
                _bootp_up, _bootp_down,
                _bootp_conv, _bootp_default
        },
        {
                "dhcp",
                _dhcp_up, _dhcp_down,
                _dhcp_conv, _dhcp_default
        },
        {
                "ipv4ll",
                _ipv4ll_up, _ipv4ll_down,
                _ipv4ll_conv, _ipv4ll_default
        },
        {
                "loopback",
                _loopback_up, _loopback_down,
                _loopback_conv, _loopback_default
        },
        {
                "manual",
                _manual_up, _manual_down,
                _manual_conv, _manual_default
        },
        {
                "ppp",
                _ppp_up, _ppp_down,
                _ppp_conv, _ppp_default
        },
        {
                "static",
                _static_up, _static_down,
                _static_conv, _static_default
        },
        {
                "tunnel",
                _tunnel_up, _tunnel_down,
                _tunnel_conv, _tunnel_default
        },
        {
                "wvdial",
                _wvdial_up, _wvdial_down,
                _wvdial_conv, _wvdial_default
        },
};

address_family addr_inet = {
        "inet",
        sizeof(methods)/sizeof(struct method),
        methods
};
