#!/bin/sh
cd ..;xgettext --add-comments=TRANSLATORS --force-po --from-code=iso-8859-1 --output=po/meteovache_pi.pot --keyword=_ --width=80 --files-from=po/POTFILES.in
