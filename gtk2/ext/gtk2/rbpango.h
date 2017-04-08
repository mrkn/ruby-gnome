/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "ruby.h"
#define PANGO_ENABLE_ENGINE
#define PANGO_ENABLE_BACKEND
#include <pango/pango.h>

#ifdef HAVE_FREETYPE2
#include <pango/pangofc-font.h>
#endif

#include "rbgobject.h"
#include "rbpangoconversions.h"
#include <pango/pangocairo.h>

#ifdef HAVE_RB_CAIRO_H
#include <rb_cairo.h>
#endif

#ifndef PANGO_TYPE_ITEM
#define PANGO_TYPE_ITEM (pango_item_get_type())
#endif
#define PANGO_TYPE_ANALYSIS (pango_analysis_get_type())
#define PANGO_TYPE_LOG_ATTR (pango_log_attr_get_type())
#ifndef PANGO_TYPE_LAYOUT_ITER
#define PANGO_TYPE_LAYOUT_ITER (pango_layout_iter_get_type())
#endif
#ifndef PANGO_TYPE_LAYOUT_LINE
#define PANGO_TYPE_LAYOUT_LINE (pango_layout_line_get_type())
#endif
#define PANGO_TYPE_RECTANGLE (pango_rectangle_get_type())
#define PANGO_TYPE_ATTR_ITERATOR (pango_attr_iter_get_type())
#define PANGO_TYPE_COVERAGE (pango_coverage_get_type())
#define PANGO_TYPE_GLYPH_INFO (pango_glyph_info_get_type())
#ifndef PANGO_TYPE_GLYPH_ITEM
#  define PANGO_TYPE_GLYPH_ITEM (pango_glyph_item_get_type())
#endif
#define PANGO_TYPE_SCRIPT_ITER (pango_script_iter_get_type())

#define ATTR2RVAL(attr) (pango_make_attribute(attr))
#define RVAL2ATTR(attr) (pango_get_attribute(attr))
#define ATTRTYPE2CLASS(attr_type) (pango_get_attribute_klass(attr_type))
#define RBPANGO_ADD_ATTRIBUTE(type, klass) (pango_add_attribute(type, klass))