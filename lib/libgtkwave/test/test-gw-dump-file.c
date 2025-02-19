#include <gtkwave.h>

static void test_blackout_regions(void)
{
    GwBlackoutRegions *regions = gw_blackout_regions_new();

    GwDumpFile *dump_file = g_object_new(GW_TYPE_DUMP_FILE, "blackout-regions", regions, NULL);
    g_object_unref(regions);

    g_assert_true(GW_IS_BLACKOUT_REGIONS(gw_dump_file_get_blackout_regions(dump_file)));
    g_assert_true(gw_dump_file_get_blackout_regions(dump_file) == regions);

    g_object_unref(dump_file);

    // get_blackout_regions must always return a valid object.

    dump_file = g_object_new(GW_TYPE_DUMP_FILE, NULL);

    g_assert_true(GW_IS_BLACKOUT_REGIONS(gw_dump_file_get_blackout_regions(dump_file)));

    g_object_unref(dump_file);
}

static void test_stems(void)
{
    GwStems *stems = gw_stems_new();

    GwDumpFile *dump_file = g_object_new(GW_TYPE_DUMP_FILE, "stems", stems, NULL);
    g_object_unref(stems);

    g_assert_true(GW_IS_STEMS(gw_dump_file_get_stems(dump_file)));
    g_assert_true(gw_dump_file_get_stems(dump_file) == stems);

    g_object_unref(dump_file);

    // get_stems must always return a valid object.

    dump_file = g_object_new(GW_TYPE_DUMP_FILE, NULL);

    g_assert_true(GW_IS_STEMS(gw_dump_file_get_stems(dump_file)));

    g_object_unref(dump_file);
}

static void test_find_symbols(void)
{
    GwLoader *loader = gw_vcd_loader_new();
    GwDumpFile *file = gw_loader_load(loader, "files/basic.vcd", NULL);
    g_assert_nonnull(file);
    g_object_unref(loader);

    GPtrArray *symbols;
    GError *error = NULL;

    // There are 6 symbols that end in `_alias` in basic.vcd.

    symbols = gw_dump_file_find_symbols(file, ".*_alias", &error);
    g_assert_nonnull(symbols);
    g_assert_cmpint(symbols->len, ==, 6);
    g_ptr_array_free(symbols, TRUE);

    // `gw_dump_file_find_symbol` returns an empty `GPtrArray` when no symbols
    // matched the regex.

    symbols = gw_dump_file_find_symbols(file, "doesNotExist", &error);
    g_assert_nonnull(symbols);
    g_assert_cmpint(symbols->len, ==, 0);
    g_ptr_array_free(symbols, TRUE);

    // NULL is only returned in case of an error.

    symbols = gw_dump_file_find_symbols(file, "(invalid_regex", &error);
    g_assert_null(symbols);
    g_assert_nonnull(error);
    g_error_free(error);

    g_object_unref(file);
}

int main(int argc, char *argv[])
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/dump_file/blackout_regions", test_blackout_regions);
    g_test_add_func("/dump_file/stems", test_stems);
    g_test_add_func("/dump_file/find_symbols", test_find_symbols);

    return g_test_run();
}