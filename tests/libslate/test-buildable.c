/* test-buildable.c
 *
 * Copyright 2024 Geoff Johnson <geoff.jay@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib.h>
#include "../../src/libslate/core/slate-buildable.h"

static void
test_buildable_defaults (void)
{
  const char *hcl = slate_buildable_get_hcl_default ();

  g_assert_nonnull (hcl);
  g_assert_true (g_str_has_prefix (hcl, "object"));
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/buildable/defaults", test_buildable_defaults);

  return g_test_run ();
}
