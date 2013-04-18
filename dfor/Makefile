all:  
	$(MAKE) -C src
VERSION = $(shell grep '^Version' ./README.md|head -1|awk '{print $$2}')
package:
	mkdir -p pkgs/SOURCES
	mkdir -p pkgs/BUILD
	mkdir -p pkgs/RPMS
	mkdir -p pkgs/SRPMS
	mkdir -p dfor-$(VERSION)
	cp -rf src dfor-$(VERSION)
	cp -rf etc dfor-$(VERSION)
	cp -rf dford dfor-$(VERSION)
	cp -f Makefile dfor-$(VERSION)
	cp -f README.md dfor-$(VERSION)
	tar cfvz pkgs/SOURCES/dfor.tar-$(VERSION).gz  dfor-$(VERSION)/
	rm -rf dfor-$(VERSION)/
	rpmbuild -ba --define "_topdir `pwd`/pkgs" --clean pkgs/SPECS/dfor.spec
install:
	mkdir -p ${prefix}/usr/local/etc/dfor
	mkdir -p ${prefix}/usr/local/bin
	mkdir -p ${prefix}/var/run/dfor
	mkdir -p ${prefix}/var/log/dfor
	mkdir -p ${prefix}/usr/local/lib
	cp etc/log4cpp.properties ${prefix}/usr/local/etc/dfor
	cp etc/config.yaml ${prefix}/usr/local/etc/dfor
	cp src/dfor.so ${prefix}/usr/local/lib/dfor.so
	cp dford/dford.js ${prefix}/usr/local/bin/
	/usr/bin/sqlite3 ${prefix}/var/run/dfor/cache.db < etc/create.sql
