# vim: set fileencoding=utf-8 :

# ***********************IMPORTANT NMAP LICENSE TERMS************************
# *                                                                         *
# * The Nmap Security Scanner is (C) 1996-2012 Insecure.Com LLC. Nmap is    *
# * also a registered trademark of Insecure.Com LLC.  This program is free  *
# * software; you may redistribute and/or modify it under the terms of the  *
# * GNU General Public License as published by the Free Software            *
# * Foundation; Version 2 with the clarifications and exceptions described  *
# * below.  This guarantees your right to use, modify, and redistribute     *
# * this software under certain conditions.  If you wish to embed Nmap      *
# * technology into proprietary software, we sell alternative licenses      *
# * (contact sales@insecure.com).  Dozens of software vendors already       *
# * license Nmap technology such as host discovery, port scanning, OS       *
# * detection, version detection, and the Nmap Scripting Engine.            *
# *                                                                         *
# * Note that the GPL places important restrictions on "derived works", yet *
# * it does not provide a detailed definition of that term.  To avoid       *
# * misunderstandings, we interpret that term as broadly as copyright law   *
# * allows.  For example, we consider an application to constitute a        *
# * "derivative work" for the purpose of this license if it does any of the *
# * following:                                                              *
# * o Integrates source code from Nmap                                      *
# * o Reads or includes Nmap copyrighted data files, such as                *
# *   nmap-os-db or nmap-service-probes.                                    *
# * o Executes Nmap and parses the results (as opposed to typical shell or  *
# *   execution-menu apps, which simply display raw Nmap output and so are  *
# *   not derivative works.)                                                *
# * o Integrates/includes/aggregates Nmap into a proprietary executable     *
# *   installer, such as those produced by InstallShield.                   *
# * o Links to a library or executes a program that does any of the above   *
# *                                                                         *
# * The term "Nmap" should be taken to also include any portions or derived *
# * works of Nmap.  This list is not exclusive, but is meant to clarify our *
# * interpretation of derived works with some common examples.  Our         *
# * interpretation applies only to Nmap--we don't speak for other people's  *
# * GPL works.                                                              *
# *                                                                         *
# * If you have any questions about the GPL licensing restrictions on using *
# * Nmap in non-GPL works, we would be happy to help.  As mentioned above,  *
# * we also offer alternative license to integrate Nmap into proprietary    *
# * applications and appliances.  These contracts have been sold to dozens  *
# * of software vendors, and generally include a perpetual license as well  *
# * as providing for priority support and updates.  They also fund the      *
# * continued development of Nmap.  Please email sales@insecure.com for     *
# * further information.                                                    *
# *                                                                         *
# * As a special exception to the GPL terms, Insecure.Com LLC grants        *
# * permission to link the code of this program with any version of the     *
# * OpenSSL library which is distributed under a license identical to that  *
# * listed in the included docs/licenses/OpenSSL.txt file, and distribute   *
# * linked combinations including the two. You must obey the GNU GPL in all *
# * respects for all of the code used other than OpenSSL.  If you modify    *
# * this file, you may extend this exception to your version of the file,   *
# * but you are not obligated to do so.                                     *
# *                                                                         *
# * If you received these files with a written license agreement or         *
# * contract stating terms other than the terms above, then that            *
# * alternative license agreement takes precedence over these comments.     *
# *                                                                         *
# * Source is provided to this software because we believe users have a     *
# * right to know exactly what a program is going to do before they run it. *
# * This also allows you to audit the software for security holes (none     *
# * have been found so far).                                                *
# *                                                                         *
# * Source code also allows you to port Nmap to new platforms, fix bugs,    *
# * and add new features.  You are highly encouraged to send your changes   *
# * to nmap-dev@insecure.org for possible incorporation into the main       *
# * distribution.  By sending these changes to Fyodor or one of the         *
# * Insecure.Org development mailing lists, it is assumed that you are      *
# * offering the Nmap Project (Insecure.Com LLC) the unlimited,             *
# * non-exclusive right to reuse, modify, and relicense the code.  Nmap     *
# * will always be available Open Source, but this is important because the *
# * inability to relicense code has caused devastating problems for other   *
# * Free Software projects (such as KDE and NASM).  We also occasionally    *
# * relicense the code to third parties as discussed above.  If you wish to *
# * specify special license conditions of your contributions, just say so   *
# * when you send them.                                                     *
# *                                                                         *
# * This program is distributed in the hope that it will be useful, but     *
# * WITHOUT ANY WARRANTY; without even the implied warranty of              *
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       *
# * General Public License v2.0 for more details at                         *
# * http://www.gnu.org/licenses/gpl-2.0.html , or in the COPYING file       *
# * included with Nmap.                                                     *
# *                                                                         *
# ***************************************************************************/

import os
import sys
import gtk
import array

from zenmapCore.Paths import Path


FORMAT_RGBA = 4
FORMAT_RGB  = 3


def get_pixels_for_cairo_image_surface(pixbuf):
    """
    This method return the image stride and a python array.ArrayType
    containing the icon pixels of a gtk.gdk.Pixbuf that can be used by
    cairo.ImageSurface.create_for_data() method.
    """
    data = array.ArrayType('c')
    format = pixbuf.get_rowstride() / pixbuf.get_width()

    i = 0
    j = 0
    while i < len(pixbuf.get_pixels()):

        b, g, r = pixbuf.get_pixels()[i:i+FORMAT_RGB]

        if format == FORMAT_RGBA:
            a = pixbuf.get_pixels()[i + FORMAT_RGBA - 1]
        elif format == FORMAT_RGB:
            a = '\xff'
        else:
            raise TypeError, 'unknown image format'

        data[j:j+FORMAT_RGBA] = array.ArrayType('c', [r, g, b, a])

        i += format
        j += FORMAT_RGBA

    return (FORMAT_RGBA * pixbuf.get_width(), data)


class Image:
    """
    """
    def __init__(self, path=None):
        """
        """
        self.__path = path
        self.__cache = dict()


    def set_path(self, path):
        """
        """
        self.__path = path


    def get_pixbuf(self, icon, image_type='png'):
        """
        """
        if self.__path == None:
            return False

        if icon + image_type not in self.__cache.keys():

            file = self.get_icon(icon, image_type)
            self.__cache[icon + image_type] = gtk.gdk.pixbuf_new_from_file(file)

        return self.__cache[icon + image_type]


    def get_icon(self, icon, image_type='png'):
        """
        """
        if self.__path == None:
            return False

        return os.path.join(self.__path, icon + "." + image_type)



class Pixmaps(Image):
    """
    """
    def __init__(self):
        """
        """
        Image.__init__(self, os.path.join(Path.pixmaps_dir, "radialnet"))



class Icons(Image):
    """
    """
    def __init__(self):
        """
        """
        Image.__init__(self, os.path.join(Path.pixmaps_dir, "radialnet"))



class Application(Image):
    """
    """
    def __init__(self):
        """
        """
        Image.__init__(self, os.path.join(Path.pixmaps_dir, "radialnet"))
