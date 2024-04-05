#include <check.h>
#include <stdlib.h>

#include "3DViewer_v1.0.h"

START_TEST(parser_1) {
  data_fm* value = (data_fm*)calloc(1, sizeof(data_fm));
  parser("../src/obj/cat.obj", value);
  ck_assert_float_eq_tol(value->matrix_3d.matrix[1][0], 0.0158797, 0.0000001);
  free_memory_matrix(&value->matrix_3d);
  free_polygon(value);
  free_facet(value);
}
END_TEST

START_TEST(parser_2) {
  data_fm* value = (data_fm*)calloc(1, sizeof(data_fm));
  parser("../src/obj/sword.obj", value);
  ck_assert_float_eq_tol(value->matrix_3d.matrix[1][1], 0.0262944, 0.0000001);
  free_memory_matrix(&value->matrix_3d);
  free_polygon(value);
  free_facet(value);
}
END_TEST

START_TEST(parser_3) {
  data_fm* value = (data_fm*)calloc(1, sizeof(data_fm));
  parser("../src/obj/baby.obj", value);
  ck_assert_float_eq_tol(value->matrix_3d.matrix[3][2], -0.0614053, 0.0000001);
  free_memory_matrix(&value->matrix_3d);
  free_polygon(value);
  free_facet(value);
}

START_TEST(rotation_ox) {
  data_fm* value = (data_fm*)calloc(1, sizeof(data_fm));
  parser("../src/obj/baby.obj", value);
  rotation_by_ox(&value->matrix_3d, 0.3);
  ck_assert_float_eq_tol(value->matrix_3d.matrix[3][2], -0.0633308, 0.0000001);
  free_memory_matrix(&value->matrix_3d);
  free_polygon(value);
  free_facet(value);
}

START_TEST(rotation_oy) {
  data_fm* value = (data_fm*)calloc(1, sizeof(data_fm));
  parser("../src/obj/baby.obj", value);
  rotation_by_oy(&value->matrix_3d, 0.3);
  ck_assert_float_eq_tol(value->matrix_3d.matrix[3][2], -0.0617034, 0.0000001);
  free_memory_matrix(&value->matrix_3d);
  free_polygon(value);
  free_facet(value);
}

START_TEST(rotation_oz) {
  data_fm* value = (data_fm*)calloc(1, sizeof(data_fm));
  parser("../src/obj/baby.obj", value);
  rotation_by_oy(&value->matrix_3d, 0.3);
  ck_assert_float_eq_tol(value->matrix_3d.matrix[3][0], 0.0567645, 0.0000001);
  free_memory_matrix(&value->matrix_3d);
  free_polygon(value);
  free_facet(value);
}

Suite* example_suite_create(void) {
  Suite* suite = suite_create("Example");
  TCase* tcase_core = tcase_create("Core of example");

  suite_add_tcase(suite, tcase_core);

  tcase_add_test(tcase_core, parser_1);
  tcase_add_test(tcase_core, parser_2);
  tcase_add_test(tcase_core, parser_3);
  tcase_add_test(tcase_core, rotation_ox);
  tcase_add_test(tcase_core, rotation_oy);
  tcase_add_test(tcase_core, rotation_oz);

  return suite;
}

int main(void) {
  Suite* suite = example_suite_create();
  SRunner* suite_runner = srunner_create(suite);

  srunner_run_all(suite_runner, CK_NORMAL);
  int failed_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);

  if (failed_count != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}