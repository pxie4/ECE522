import sys
import gdb

# Update module path.
dir = '/home/kvm/parsec-3.0/bin/../pkgs/libs/glib/inst/amd64-linux.gcc-pthreads/share/glib-2.0/gdb'
if not dir in sys.path:
    sys.path.insert(0, dir)

from gobject import register
register (gdb.current_objfile ())
