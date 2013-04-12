package:
	mkdir -p pkgs/SOURCES
	mkdir -p pkgs/BUILD
	mkdir -p pkgs/RPMS
	mkdir -p dfor-0.1
	cp -rf src/dfor.so dfor-0.1
	cp -rf etc dfor-0.1
	cp -rf src/Makefile dfor-0.1
	tar cfvz pkgs/SOURCES/dfor.tar.gz  dfor-0.1/
	rm -rf dfor-0.1
	rpmbuild -ba --define "_topdir `pwd`/pkgs" --clean pkgs/SPECS/dfor.spec
