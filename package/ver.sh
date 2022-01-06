
# $Header$

# Input file for: mkver.pl.  All variables must have
# "export " at the beginning.  No spaces around the
# "=".  And all values enclosed with double quotes.
# Variables may include other variables in their
# values.

export ProdName="PRODNAME"
# One word [-_.a-zA-Z0-9]

export ProdAlias="PRODALIAS"
# One word [-_.a-zA-Z0-9]

export ProdVer="1.0"
# [0-9]*.[0-9]*{.[0-9]*}{.[0-9]*}

export ProdBuild="1"
# [0-9]*

export ProdSummary="PRODSUMMARY"
# All on one line (< 80 char)

export ProdDesc="PRODDESC"
# All on one line

export ProdVendor="COMPANY"

export ProdPackager="$LOGNAME"
export ProdSupport="support\@COMPANY.com"
export ProdCopyright=""

export ProdDate=""
# 20[012][0-9]-[01][0-9]-[0123][0-9]

export ProdLicense="COPYING"
# Required

export ProdReadMe="README"
# Required

# Third Party (if any)
export ProdTPVendor=""
export ProdTPVer=""
export ProdTPCopyright=""

# Set this to latest version of mkver.pl
export MkVer="2.2"

export ProdRelServer="rel.DOMAIN.com"
export ProdRelRoot="/release/package"
export ProdRelCategory="software/ThirdParty/$ProdName"
# Generated: ProdRelDir=ProdRelRoot . /released|development/ . ProdRelCategory
# (if RELEASE=1, then use "released", else use "development")
# Generated: ProdDevDir=ProdRelRoot/development/ProdRelCategory

# Generated: ProdTag=ProdVer-ProdBuild
# (All "." converted to "-")

# Generated: ProdOS (DistVer)
#	Dist
#		Ver
# linux
# 	deb
# 	rhes
# 	cent
# 	fc
# cygwin
#	cygwin
# mswin32
#	win
#		xp
# solaris
#	sun
# darwin
#	mac

# Generated: ProdArch
# i386
# x86_64

# Output file control variables.
# The *File vars can include dir. names
# The *Header and *Footer defaults are more complete
# than what is shown here.

export envFile="ver.env"
export envHeader=""
export envFooter=""

export epmFile="ver.epm"
export epmHeader=""
export epmFooter="# %include ver.list"

export hFile="ver.h"
export hHeader=""
export hFooter=""

export javaPackage="DIR.DIR.DIR"
export javaInterface="ver"
export javaFile="ver.java"
export javaHeader=""
export javaFooter=""

export csNamespace="Supernode"
export csClass="ver"
export csFile="ver.cs"
export csHeader=""
export csFooter=""

export makFile="ver.mak"
export makHeader=""
export makFooter=""

export plFile="ver.pl"
export plHeader=""
export plFooter=""

export xmlFile="ver.xml"
export xmlHeader=""
export xmlFooter=""
