pkgname=h3xo-st
_pkgname=st
pkgver=0.8.4
pkgrel=1
pkgdesc="h3xo's suckless terminal"
url='https://gitlab.com/h3xOo/h3xo-st'
arch=('i686' 'x86_64')
license=('GPL3')
options=('zipman')
depends=('libxft')
makedepends=('ncurses' 'libxext' 'git')
optdepends=('dmenu: feed urls to dmenu')
source=('git+https://gitlab.com/h3xOo/h3xo-st')
sha1sums=('SKIP')

provides=($_pkgname)
conflicts=($_pkgname)

pkgver() {
	cd "${pkgname}"
	printf "%s.r%s.%s" "$(awk '/^VERSION =/ {print $3}' config.mk)" \
		"$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

prepare() {
	cd $srcdir/${pkgname}
	# skip terminfo which conflicts with ncurses
	sed -i '/tic /d' Makefile
}

build() {
	cd "${pkgname}"
	make
}

package() {
	cd "${pkgname}"
	make PREFIX=/usr DESTDIR="${pkgdir}" install
}
