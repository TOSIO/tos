Sample init scripts and service configuration for toschaind
==========================================================

Sample scripts and configuration files for systemd, Upstart and OpenRC
can be found in the contrib/init folder.

    contrib/init/toschaind.service:    systemd service unit configuration
    contrib/init/toschaind.openrc:     OpenRC compatible SysV style init script
    contrib/init/toschaind.openrcconf: OpenRC conf.d file
    contrib/init/toschaind.conf:       Upstart service configuration file
    contrib/init/toschaind.init:       CentOS compatible SysV style init script

Service User
---------------------------------

All three Linux startup configurations assume the existence of a "toschain" user
and group.  They must be created before attempting to use these scripts.
The OS X configuration assumes toschaind will be set up for the current user.

Configuration
---------------------------------

At a bare minimum, toschaind requires that the rpcpassword setting be set
when running as a daemon.  If the configuration file does not exist or this
setting is not set, toschaind will shutdown promptly after startup.

This password does not have to be remembered or typed as it is mostly used
as a fixed token that toschaind and client programs read from the configuration
file, however it is recommended that a strong and secure password be used
as this password is security critical to securing the wallet should the
wallet be enabled.

If toschaind is run with the "-server" flag (set by default), and no rpcpassword is set,
it will use a special cookie file for authentication. The cookie is generated with random
content when the daemon starts, and deleted when it exits. Read access to this file
controls who can access it through RPC.

By default the cookie is stored in the data directory, but it's location can be overridden
with the option '-rpccookiefile'.

This allows for running toschaind without having to do any manual configuration.

`conf`, `pid`, and `wallet` accept relative paths which are interpreted as
relative to the data directory. `wallet` *only* supports relative paths.

For an example configuration file that describes the configuration settings,
see `contrib/debian/examples/toschain.conf`.

Paths
---------------------------------

### Linux

All three configurations assume several paths that might need to be adjusted.

Binary:              `/usr/bin/toschaind`  
Configuration file:  `/etc/toschain/toschain.conf`  
Data directory:      `/var/lib/toschaind`  
PID file:            `/var/run/toschaind/toschaind.pid` (OpenRC and Upstart) or `/var/lib/toschaind/toschaind.pid` (systemd)  
Lock file:           `/var/lock/subsys/toschaind` (CentOS)  

The configuration file, PID directory (if applicable) and data directory
should all be owned by the toschain user and group.  It is advised for security
reasons to make the configuration file and data directory only readable by the
toschain user and group.  Access to toschain-cli and other toschaind rpc clients
can then be controlled by group membership.

### Mac OS X

Binary:              `/usr/local/bin/toschaind`  
Configuration file:  `~/Library/Application Support/Toschain/toschain.conf`  
Data directory:      `~/Library/Application Support/Toschain`  
Lock file:           `~/Library/Application Support/Toschain/.lock`  

Installing Service Configuration
-----------------------------------

### systemd

Installing this .service file consists of just copying it to
/usr/lib/systemd/system directory, followed by the command
`systemctl daemon-reload` in order to update running systemd configuration.

To test, run `systemctl start toschaind` and to enable for system startup run
`systemctl enable toschaind`

NOTE: When installing for systemd in Debian/Ubuntu the .service file needs to be copied to the /lib/systemd/system directory instead.

### OpenRC

Rename toschaind.openrc to toschaind and drop it in /etc/init.d.  Double
check ownership and permissions and make it executable.  Test it with
`/etc/init.d/toschaind start` and configure it to run on startup with
`rc-update add toschaind`

### Upstart (for Debian/Ubuntu based distributions)

Upstart is the default init system for Debian/Ubuntu versions older than 15.04. If you are using version 15.04 or newer and haven't manually configured upstart you should follow the systemd instructions instead.

Drop toschaind.conf in /etc/init.  Test by running `service toschaind start`
it will automatically start on reboot.

NOTE: This script is incompatible with CentOS 5 and Amazon Linux 2014 as they
use old versions of Upstart and do not supply the start-stop-daemon utility.

### CentOS

Copy toschaind.init to /etc/init.d/toschaind. Test by running `service toschaind start`.

Using this script, you can adjust the path and flags to the toschaind program by
setting the TOSCHAIND and FLAGS environment variables in the file
/etc/sysconfig/toschaind. You can also use the DAEMONOPTS environment variable here.

### Mac OS X

Copy org.toschain.toschaind.plist into ~/Library/LaunchAgents. Load the launch agent by
running `launchctl load ~/Library/LaunchAgents/org.toschain.toschaind.plist`.

This Launch Agent will cause toschaind to start whenever the user logs in.

NOTE: This approach is intended for those wanting to run toschaind as the current user.
You will need to modify org.toschain.toschaind.plist if you intend to use it as a
Launch Daemon with a dedicated toschain user.

Auto-respawn
-----------------------------------

Auto respawning is currently only configured for Upstart and systemd.
Reasonable defaults have been chosen but YMMV.
