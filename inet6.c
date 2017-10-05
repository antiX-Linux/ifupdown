#include <stddef.h>
#include "header.h"


#include "archcommon.h"
#include "archlinux.h"


static option_default _auto_default[] = {
	{ "accept_ra", "2" },
	{ "request_prefix", "0" },
	{ "ll-attempts", "60" },
	{ "ll-interval", "0.1" },
	{ NULL, NULL }
};
static conversion _auto_conv[] = {
	{ NULL, NULL, NULL, 0, NULL }
};
static int _auto_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/sbin/modprobe -q net-pf-10 > /dev/null 2>&1 || true # ignore failure.", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("-[[/sbin/sysctl -q -e -w net.ipv6.conf.%iface/.//%.use_tempaddr=%privext%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("-/sbin/sysctl -q -e -w net.ipv6.conf.%iface/.//%.accept_ra=%accept_ra%", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("-/sbin/sysctl -q -e -w net.ipv6.conf.%iface/.//%.autoconf=1", ifd, exec) && !ignore_failures) return 0;
}
if ( var_set("accept_ra", ifd) && !var_true("accept_ra", ifd) ) {
  if (!execute("/bin/ip addr flush dev %iface% mngtmpaddr", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip link set dev %iface% up", ifd, exec) && !ignore_failures) return 0;
}
if ( var_true("dhcp", ifd) && execable("/lib/ifupdown/wait-for-ll6.sh") ) {
  if (!execute("/lib/ifupdown/wait-for-ll6.sh", ifd, exec) && !ignore_failures) return 0;
}
if ( var_true("dhcp", ifd) && execable("/sbin/dhclient") && var_true("request_prefix", ifd) ) {
  if (!execute("/sbin/dhclient -6 -P -pf /run/dhclient6.%iface%.pid -lf /var/lib/dhcp/dhclient6.%iface%.leases -I -df /var/lib/dhcp/dhclient.%iface%.leases %iface%", ifd, exec) && !ignore_failures) return 0;
}
else if ( var_true("dhcp", ifd) && execable("/sbin/dhclient") ) {
  if (!execute("/sbin/dhclient -6 -S -pf /run/dhclient6.%iface%.pid -lf /var/lib/dhcp/dhclient6.%iface%.leases -I -df /var/lib/dhcp/dhclient.%iface%.leases %iface%", ifd, exec) && !ignore_failures) return 0;
}
else if ( var_true("dhcp", ifd) ) {
  if (!execute("echo 'No DHCPv6 client software found!' >/dev/stderr; false", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _auto_down(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/bin/ip -6 addr flush dev %iface% scope global", ifd, exec) && !ignore_failures) return 0;
}
if ( iface_is_link() ) {
  if (!execute("/bin/ip link set dev %iface% down", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _loopback_default[] = {
	{ NULL, NULL }
};
static conversion _loopback_conv[] = {
	{ NULL, NULL, NULL, 0, NULL }
};
static int _loopback_up(interface_defn *ifd, execfn *exec) {
if ( !iface_is_lo() ) {
  if (!execute("-/bin/ip link set dev %iface% up 2>/dev/null", ifd, exec) && !ignore_failures) return 0;
}
if ( !iface_is_lo() ) {
  if (!execute("-/bin/ip addr add dev %iface% ::1 2>/dev/null", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _loopback_down(interface_defn *ifd, execfn *exec) {
if ( !iface_is_lo() ) {
  if (!execute("-/bin/ip addr del dev %iface% ::1 2>/dev/null", ifd, exec) && !ignore_failures) return 0;
}
if ( !iface_is_lo() ) {
  if (!execute("-/bin/ip link set dev %iface% down 2>/dev/null", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _static_default[] = {
	{ "autoconf", "0" },
	{ "preferred-lifetime", "" },
	{ "dad-attempts", "60" },
	{ "dad-interval", "0.1" },
	{ NULL, NULL }
};
static conversion _static_conv[] = {
	{ "hwaddress", NULL, cleanup_hwaddress, 0, NULL },
	{ "preferred-lifetime", NULL, set_preferred_lft, 0, NULL },
	{ "address", "netmask?", get_token, 3, (char * []){"/", "1", ""} },
	{ "address", NULL, get_token, 3, (char * []){"/", "0", ""} },
	{ "gateway", "accept_ra?", if_set, 1, (char * []){"0"} },
	{ NULL, NULL, NULL, 0, NULL }
};
static int _static_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/sbin/modprobe -q net-pf-10 > /dev/null 2>&1 || true # ignore failure.", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("-[[/sbin/sysctl -q -e -w net.ipv6.conf.%iface/.//%.use_tempaddr=%privext%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("-[[/sbin/sysctl -q -e -w net.ipv6.conf.%iface/.//%.accept_ra=%accept_ra%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("-[[/sbin/sysctl -q -e -w net.ipv6.conf.%iface/.//%.autoconf=%autoconf%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("-[[/sbin/sysctl -q -e -w net.ipv6.conf.%iface/.//%.mtu=%mtu%]]", ifd, exec) && !ignore_failures) return 0;
}
if ( var_set("accept_ra", ifd) && !var_true("accept_ra", ifd) ) {
  if (!execute("/bin/ip addr flush dev %iface% mngtmpaddr", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip link set dev %iface% [[address %hwaddress%]] up", ifd, exec) && !ignore_failures) return 0;
}
if ( !var_set("dad-attempts", ifd) || var_true("dad-attempts", ifd) ) {
  if (!execute("/bin/ip -6 addr add %address%[[/%netmask%]] [[scope %scope%]] dev %iface% [[preferred_lft %preferred-lifetime%]]", ifd, exec) && !ignore_failures) return 0;
}
if ( var_set("dad-attempts", ifd) && !var_true("dad-attempts", ifd) ) {
  if (!execute("/bin/ip -6 addr add %address%[[/%netmask%]] [[scope %scope%]] dev %iface% [[preferred_lft %preferred-lifetime%]] nodad", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[ /bin/ip -6 route add default via %gateway% [[metric %metric%]] dev %iface% onlink ]]", ifd, exec) && !ignore_failures) return 0;
}
if ( !no_act && execable("/lib/ifupdown/settle-dad.sh") && (var_true("dad-attempts", ifd)) ) {
  if (!execute("/lib/ifupdown/settle-dad.sh", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _static_down(interface_defn *ifd, execfn *exec) {
{
  if (!execute("[[ /bin/ip -6 route del default via %gateway% [[metric %metric%]] dev %iface% ]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip -6 addr del %address%[[/%netmask%]] [[scope %scope%]] dev %iface%", ifd, exec) && !ignore_failures) return 0;
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
  if (!execute("-[[/sbin/sysctl -q -e -w net.ipv6.conf.%iface/.//%.mtu=%mtu%]]", ifd, exec) && !ignore_failures) return 0;
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
if ( iface_is_link() || !do_all ) {
  if (!execute("/bin/ip link set dev %iface% down 2>/dev/null || true", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _dhcp_default[] = {
	{ "accept_ra", "1" },
	{ "request_prefix", "0" },
	{ "ll-attempts", "60" },
	{ "ll-interval", "0.1" },
	{ NULL, NULL }
};
static conversion _dhcp_conv[] = {
	{ "hwaddress", NULL, cleanup_hwaddress, 0, NULL },
	{ NULL, NULL, NULL, 0, NULL }
};
static int _dhcp_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/sbin/modprobe -q net-pf-10 > /dev/null 2>&1 || true # ignore failure.", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("-[[/sbin/sysctl -q -e -w net.ipv6.conf.%iface/.//%.accept_ra=%accept_ra%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("-[[/sbin/sysctl -q -e -w net.ipv6.conf.%iface/.//%.autoconf=%autoconf%]]", ifd, exec) && !ignore_failures) return 0;
}
if ( var_set("accept_ra", ifd) && !var_true("accept_ra", ifd) ) {
  if (!execute("/bin/ip addr flush dev %iface% mngtmpaddr", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip link set dev %iface% [[address %hwaddress%]] up", ifd, exec) && !ignore_failures) return 0;
}
if ( execable("/lib/ifupdown/wait-for-ll6.sh") ) {
  if (!execute("/lib/ifupdown/wait-for-ll6.sh", ifd, exec) && !ignore_failures) return 0;
}
if ( execable("/sbin/dhclient") && var_true("request_prefix", ifd) ) {
  if (!execute("/sbin/dhclient -6 -pf /run/dhclient6.%iface%.pid -lf /var/lib/dhcp/dhclient6.%iface%.leases -I -P -N -df /var/lib/dhcp/dhclient.%iface%.leases %iface%", ifd, exec) && !ignore_failures) return 0;
}
else if ( execable("/sbin/dhclient") ) {
  if (!execute("/sbin/dhclient -6 -pf /run/dhclient6.%iface%.pid -lf /var/lib/dhcp/dhclient6.%iface%.leases -I -df /var/lib/dhcp/dhclient.%iface%.leases %iface%", ifd, exec) && !ignore_failures) return 0;
}
else if ( 1 ) {
  if (!execute("echo 'No DHCPv6 client software found!' >/dev/stderr; false", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _dhcp_down(interface_defn *ifd, execfn *exec) {
if ( execable("/sbin/dhclient") ) {
  if (!execute("/sbin/dhclient -6 -r -pf /run/dhclient6.%iface%.pid -lf /var/lib/dhcp/dhclient6.%iface%.leases -I -df /var/lib/dhcp/dhclient.%iface%.leases %iface%", ifd, exec) && !ignore_failures) return 0;
}
else if ( 1 ) {
  if (!execute("echo 'No DHCPv6 client software found!' >/dev/stderr; false", ifd, exec) && !ignore_failures) return 0;
}
if ( iface_is_link() ) {
  if (!execute("/bin/ip link set dev %iface% down", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _v4tunnel_default[] = {
	{ "preferred-lifetime", "" },
	{ NULL, NULL }
};
static conversion _v4tunnel_conv[] = {
	{ "preferred-lifetime", NULL, set_preferred_lft, 0, NULL },
	{ NULL, NULL, NULL, 0, NULL }
};
static int _v4tunnel_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/sbin/modprobe -q net-pf-10 > /dev/null 2>&1 || true # ignore failure.", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip tunnel add %iface% mode sit remote %endpoint% [[local %local%]] \
       [[ttl %ttl%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip link set %iface% up [[mtu %mtu%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[ /bin/ip addr add %address%[[/%netmask%]] dev %iface% [[preferred_lft %preferred-lifetime%]] ]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[ /bin/ip route add %gateway% dev %iface% ]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("[[ /bin/ip route add ::/0 via %gateway% [[metric %metric%]] dev %iface% onlink ]]", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _v4tunnel_down(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/bin/ip tunnel del %iface%", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static option_default _6to4_default[] = {
	{ "preferred-lifetime", "" },
	{ NULL, NULL }
};
static conversion _6to4_conv[] = {
	{ "local", "hexaddress", make_hex_address, 0, NULL },
	{ "preferred-lifetime", NULL, set_preferred_lft, 0, NULL },
	{ NULL, NULL, NULL, 0, NULL }
};
static int _6to4_up(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/sbin/modprobe -q net-pf-10 > /dev/null 2>&1 || true # ignore failure.", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip tunnel add %iface% mode sit remote any local %local% \
       [[ttl %ttl%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip link set %iface% up [[mtu %mtu%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip addr add 2002:%hexaddress%::1/16 dev %iface% [[preferred_lft %preferred-lifetime%]]", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip route add 2000::/3 via ::192.88.99.1 [[metric %metric%]] dev %iface%", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static int _6to4_down(interface_defn *ifd, execfn *exec) {
{
  if (!execute("/bin/ip -6 route flush dev %iface%", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip link set dev %iface% down", ifd, exec) && !ignore_failures) return 0;
}
{
  if (!execute("/bin/ip tunnel del %iface%", ifd, exec) && !ignore_failures) return 0;
}
return 1;
}
static method methods[] = {
        {
                "6to4",
                _6to4_up, _6to4_down,
                _6to4_conv, _6to4_default
        },
        {
                "auto",
                _auto_up, _auto_down,
                _auto_conv, _auto_default
        },
        {
                "dhcp",
                _dhcp_up, _dhcp_down,
                _dhcp_conv, _dhcp_default
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
                "static",
                _static_up, _static_down,
                _static_conv, _static_default
        },
        {
                "v4tunnel",
                _v4tunnel_up, _v4tunnel_down,
                _v4tunnel_conv, _v4tunnel_default
        },
};

address_family addr_inet6 = {
        "inet6",
        sizeof(methods)/sizeof(struct method),
        methods
};
