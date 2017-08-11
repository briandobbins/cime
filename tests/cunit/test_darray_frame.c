/*
 * Tests for PIO distributed arrays. This test is a C version of some
 * fortran tests in pio_decomp_frame_tests.f90.
 *
 * @author Ed Hartnett
 * @date 8/11/17
 */
#include <pio.h>
#include <pio_internal.h>
#include <pio_tests.h>

/* The number of tasks this test should run on. */
#define TARGET_NTASKS 4

/* The minimum number of tasks this test should run on. */
#define MIN_NTASKS 4

/* The name of this test. */
#define TEST_NAME "test_darray_frame"

/* The number of dimensions in the example data. In this test, we
 * are using three-dimensional data. */
#define NDIM3 3

/* But sometimes we need arrays of the non-record dimensions. */
#define NDIM2 2

/* These are the dimension values used in a typical input file to
 * ESMCI/cime. */
/* #define TIME_LEN 12 */
/* #define LAT_LEN 94 */
/* #define LON_LEN 192 */

/* Here's a shorter version for a simpler test. */
#define TIME_LEN_SHORT 5
#define LAT_LEN_SHORT 2
#define LON_LEN_SHORT 3

/* The number of timesteps of data to write. */
#define NUM_TIMESTEPS 2

/* The names of variable in the netCDF output files. */
#define VAR_NAME "prc"

/**
 * Test the darray functionality. Create a netCDF file with 3
 * dimensions and 1 PIO_INT variable, and use darray to write some
 * data. There are no unlimited dimensions.
 *
 * @param iosysid the IO system ID.
 * @param num_iotypes the number of IOTYPES available in this build.
 * @param iotype array of available iotypes.
 * @param my_rank rank of this task.
 * @param pio_type the type of the data.
 * @returns 0 for success, error code otherwise.
 */
int test_frame_simple(int iosysid, int num_iotypes, int *iotype, int my_rank,
                      int pio_type)
{
    int ioid;
    int dim_len_2d[NDIM2] = {LAT_LEN_SHORT, LON_LEN_SHORT};
    /* PIO_Offset arraylen = 4; */
    /* void *fillvalue; */
    /* void *test_data; */
    /* void *test_data_in; */
    /* int fillvalue_int = NC_FILL_INT; */
    /* int test_data_int[arraylen]; */
    /* int test_data_int_in[arraylen]; */
    /* float fillvalue_float = NC_FILL_FLOAT; */
    /* float test_data_float[arraylen]; */
    /* float test_data_float_in[arraylen]; */
    /* double fillvalue_double = NC_FILL_DOUBLE; */
    /* double test_data_double[arraylen]; */
    /* double test_data_double_in[arraylen]; */
    int ret;       /* Return code. */

    /* Decompose the data over the tasks. */
    if ((ret = create_decomposition_2d(TARGET_NTASKS, my_rank, iosysid, dim_len_2d,
                                       &ioid, PIO_INT)))
        return ret;

    /* /\* Initialize some data. *\/ */
    /* for (int f = 0; f < arraylen; f++) */
    /* { */
    /*     test_data_int[f] = my_rank * 10 + f; */
    /*     test_data_float[f] = my_rank * 10 + f + 0.5; */
    /*     test_data_double[f] = my_rank * 100000 + f + 0.5; */
    /* } */

    /* Use PIO to create the example file in each of the four
     * available ways. */
    /* for (int fmt = 0; fmt < num_iotypes; fmt++)  */
    for (int fmt = 0; fmt < 1; fmt++)
    {
        int ncid;      /* The ncid of the netCDF file. */
        int ncid2;     /* The ncid of the re-opened netCDF file. */
        char filename[PIO_MAX_NAME + 1]; /* Name for the output files. */
        int dimids[NDIM3];      /* The dimension IDs. */
        int dim_len[NDIM3] = {TIME_LEN_SHORT, LAT_LEN_SHORT, LON_LEN_SHORT};
        char dim_name[NDIM3][PIO_MAX_NAME + 1] = {"time", "lat", "lon"};
        int varid;     /* The ID of the netCDF varable. */
        
        /* Create the filename. */
        sprintf(filename, "simple_frame_%s_iotype_%d.nc", TEST_NAME, iotype[fmt]);

        /* Create the netCDF output file. */
        printf("rank: %d Creating sample file %s\n", my_rank, filename);
        if ((ret = PIOc_createfile(iosysid, &ncid, &iotype[fmt], filename, PIO_CLOBBER)))
            ERR(ret);

        /* Define netCDF dimensions and variable. */
        for (int d = 0; d < NDIM3; d++)
            if ((ret = PIOc_def_dim(ncid, dim_name[d], (PIO_Offset)dim_len[d], &dimids[d])))
                ERR(ret);

        /* Define a variable. */
        if ((ret = PIOc_def_var(ncid, VAR_NAME, PIO_INT, NDIM3, dimids, &varid)))
            ERR(ret);

        /* End define mode. */
        if ((ret = PIOc_enddef(ncid)))
            ERR(ret);

        /* /\* Set the value of the record dimension. *\/ */
        /* if ((ret = PIOc_setframe(ncid, varid, 0))) */
        /*     ERR(ret); */

        /* int frame = 0; */
        /* int flushtodisk = test_multi - 1; */
        /* if (!test_multi) */
        /* { */
        /*     /\* These should not work. *\/ */
        /*     if (PIOc_write_darray(ncid + TEST_VAL_42, varid, ioid, arraylen, test_data, fillvalue) != PIO_EBADID) */
        /*         ERR(ERR_WRONG); */
        /*     if (PIOc_write_darray(ncid, varid, ioid + TEST_VAL_42, arraylen, test_data, fillvalue) != PIO_EBADID) */
        /*         ERR(ERR_WRONG); */
        /*     if (PIOc_write_darray(ncid, varid, ioid, arraylen - 1, test_data, fillvalue) != PIO_EINVAL) */
        /*         ERR(ERR_WRONG); */
        /*     if (PIOc_write_darray(ncid, TEST_VAL_42, ioid, arraylen, test_data, fillvalue) != PIO_ENOTVAR) */
        /*         ERR(ERR_WRONG); */
        /*     if (PIOc_write_darray(ncid, varid2, ioid, arraylen, test_data, fillvalue) != PIO_EINVAL) */
        /*         ERR(ERR_WRONG); */

        /*     /\* Write the data. *\/ */
        /*     if ((ret = PIOc_write_darray(ncid, varid, ioid, arraylen, test_data, fillvalue))) */
        /*         ERR(ret); */
        /* } */
        /* else */
        /* { */
        /*     int varid_big = NC_MAX_VARS + TEST_VAL_42; */

        /*     /\* These will not work. *\/ */
        /*     if (PIOc_write_darray_multi(ncid + TEST_VAL_42, &varid, ioid, 1, arraylen, test_data, &frame, */
        /*                                 fillvalue, flushtodisk) != PIO_EBADID) */
        /*         ERR(ERR_WRONG); */
        /*     if (PIOc_write_darray_multi(ncid, NULL, ioid, 1, arraylen, test_data, &frame, */
        /*                                 fillvalue, flushtodisk) != PIO_EINVAL) */
        /*         ERR(ERR_WRONG); */
        /*     if (PIOc_write_darray_multi(ncid, &varid, ioid + TEST_VAL_42, 1, arraylen, test_data, &frame, */
        /*                                 fillvalue, flushtodisk) != PIO_EBADID) */
        /*         ERR(ERR_WRONG); */
        /*     if (PIOc_write_darray_multi(ncid, &varid, ioid, -1, arraylen, test_data, &frame, */
        /*                                 fillvalue, flushtodisk) != PIO_EINVAL) */
        /*         ERR(ERR_WRONG); */
        /*     if (PIOc_write_darray_multi(ncid, &varid_big, ioid, 1, arraylen, test_data, &frame, */
        /*                                 fillvalue, flushtodisk) != PIO_ENOTVAR) */
        /*         ERR(ERR_WRONG); */
        /*     if (PIOc_write_darray_multi(ncid, &wrong_varid, ioid, 1, arraylen, test_data, &frame, */
        /*                                 fillvalue, flushtodisk) != PIO_ENOTVAR) */
        /*         ERR(ERR_WRONG); */

        /*     /\* Write the data with the _multi function. *\/ */
        /*     if ((ret = PIOc_write_darray_multi(ncid, &varid, ioid, 1, arraylen, test_data, &frame, */
        /*                                        fillvalue, flushtodisk))) */
        /*         ERR(ret); */
        /* } */

        /* Close the netCDF file. */
        if ((ret = PIOc_closefile(ncid)))
            ERR(ret);

        /* Reopen the file. */
        if ((ret = PIOc_openfile(iosysid, &ncid2, &iotype[fmt], filename, PIO_NOWRITE)))
            ERR(ret);

        /* /\* These should not work. *\/ */
        /* if (PIOc_read_darray(ncid2 + TEST_VAL_42, varid, ioid, arraylen, */
        /*                      test_data_in) != PIO_EBADID) */
        /*     ERR(ERR_WRONG); */
        /* if (PIOc_read_darray(ncid2, varid, ioid + TEST_VAL_42, arraylen, */
        /*                      test_data_in) != PIO_EBADID) */
        /*     ERR(ERR_WRONG); */

        /* /\* Set the record number. *\/ */
        /* if ((ret = PIOc_setframe(ncid2, varid, 0))) */
        /*     ERR(ret); */

        /* /\* Read the data. *\/ */
        /* if ((ret = PIOc_read_darray(ncid2, varid, ioid, arraylen, test_data_in))) */
        /*     ERR(ret); */

        /* /\* Check the results. *\/ */
        /* for (int f = 0; f < arraylen; f++) */
        /* { */
        /*     switch (pio_type) */
        /*     { */
        /*     case PIO_INT: */
        /*         if (test_data_int_in[f] != test_data_int[f]) */
        /*             return ERR_WRONG; */
        /*         break; */
        /*     case PIO_FLOAT: */
        /*         if (test_data_float_in[f] != test_data_float[f]) */
        /*             return ERR_WRONG; */
        /*         break; */
        /*     case PIO_DOUBLE: */
        /*         if (test_data_double_in[f] != test_data_double[f]) */
        /*             return ERR_WRONG; */
        /*         break; */
        /*     default: */
        /*         ERR(ERR_WRONG); */
        /*     } */
        /* } */

        /* /\* Try to write, but it won't work, because we opened file read-only. *\/ */
        /* if (!test_multi) */
        /* { */
        /*     if (PIOc_write_darray(ncid2, varid, ioid, arraylen, test_data, fillvalue) != PIO_EPERM) */
        /*         ERR(ERR_WRONG); */
        /* } */
        /* else */
        /* { */
        /*     if (PIOc_write_darray_multi(ncid2, &varid, ioid, 1, arraylen, test_data, &frame, */
        /*                                 fillvalue, flushtodisk) != PIO_EPERM) */
        /*         ERR(ERR_WRONG); */
        /* } */

        /* Close the netCDF file. */
        printf("%d Closing the sample data file...\n", my_rank);
        if ((ret = PIOc_closefile(ncid2)))
            ERR(ret);
    } /* next iotype */

    /* Free the PIO decomposition. */
    if ((ret = PIOc_freedecomp(iosysid, ioid)))
        ERR(ret);
    return PIO_NOERR;
}

/* Run tests for darray functions. */
int main(int argc, char **argv)
{
#define NUM_REARRANGERS_TO_TEST 2
    int rearranger[NUM_REARRANGERS_TO_TEST] = {PIO_REARR_BOX, PIO_REARR_SUBSET};
    int my_rank;
    int ntasks;
    int num_iotypes; /* Number of PIO netCDF iotypes in this build. */
    int iotype[NUM_IOTYPES]; /* iotypes for the supported netCDF IO iotypes. */
    MPI_Comm test_comm; /* A communicator for this test. */
    int ret;         /* Return code. */

    /* Initialize test. */
    if ((ret = pio_test_init2(argc, argv, &my_rank, &ntasks, MIN_NTASKS,
                              MIN_NTASKS, 3, &test_comm)))
        ERR(ERR_INIT);

    if ((ret = PIOc_set_iosystem_error_handling(PIO_DEFAULT, PIO_RETURN_ERROR, NULL)))
        return ret;

    /* Only do something on max_ntasks tasks. */
    if (my_rank < TARGET_NTASKS)
    {
        int iosysid;  /* The ID for the parallel I/O system. */
        int ioproc_stride = 1;    /* Stride in the mpi rank between io tasks. */
        int ioproc_start = 0;     /* Zero based rank of first processor to be used for I/O. */
        int ret;      /* Return code. */

        /* Figure out iotypes. */
        if ((ret = get_iotypes(&num_iotypes, iotype)))
            ERR(ret);
        printf("Runnings tests for %d iotypes\n", num_iotypes);

        for (int r = 0; r < NUM_REARRANGERS_TO_TEST; r++)
        {
            /* Initialize the PIO IO system. This specifies how
             * many and which processors are involved in I/O. */
            if ((ret = PIOc_Init_Intracomm(test_comm, TARGET_NTASKS, ioproc_stride,
                                           ioproc_start, rearranger[r], &iosysid)))
                return ret;

            /* Run tests. */
            printf("%d Running tests...\n", my_rank);
            if ((ret = test_frame_simple(iosysid, num_iotypes, iotype, my_rank, test_comm)))
                return ret;

            /* Finalize PIO system. */
            if ((ret = PIOc_finalize(iosysid)))
                return ret;
        } /* next rearranger */
    } /* endif my_rank < TARGET_NTASKS */

    /* Finalize the MPI library. */
    printf("%d %s Finalizing...\n", my_rank, TEST_NAME);
    if ((ret = pio_test_finalize(&test_comm)))
        return ret;

    printf("%d %s SUCCESS!!\n", my_rank, TEST_NAME);
    return 0;
}
