# This will check for ANY domain join
from winim/com import GetObject
try:
    discard GetObject(obf("LDAP://RootDSE"))
except:
    # quit if non domain joined
    quit()
finally:
    # continue if domain joined
    when defined(verbose):
        echo ""
