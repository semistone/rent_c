all:  
	make -C src
package:
	mkdir -p pkgs/SOURCES
	mkdir -p pkgs/BUILD
	mkdir -p pkgs/RPMS
	mkdir -p dfor-0.1
	cp -rf src dfor-0.1
	cp -rf etc dfor-0.1
	cp -rf Makefile dfor-0.1
	tar cfvz pkgs/SOURCES/dfor.tar.gz  dfor-0.1/
	rm -rf dfor-0.1
	rpmbuild -ba --define "_topdir `pwd`/pkgs" --clean pkgs/SPECS/dfor.spec
install:
	mkdir -p ${prefix}/usr/local/etc/dfor
	mkdir -p ${prefix}/var/run/dfor
	mkdir -p ${prefix}/var/log/dfor
	mkdir -p ${prefix}/usr/local/lib
	cp etc/log4cpp.properties ${prefix}/usr/local/etc/dfor
	cp src/dfor.so ${prefix}/usr/local/lib/dfor.so
