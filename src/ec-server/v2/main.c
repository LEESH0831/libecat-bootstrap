

#include "ec-server/v2/ec_system.h"
#include "ec-server/v2/sock.h"
#include "ec-server/v2/utils.h"



char IOmap[4096];

OSAL_THREAD_HANDLE thread1;

int expectedWKC;

boolean needlf;

volatile int wkc;

boolean inOP;

uint8 currentgroup = 0;

boolean forceByteAlignment = FALSE;

int debug_mode = 0;


// margin degrees to make homing reliable

int HOMING_MARGIN_DEGREES;


int WORKING_COUNTER_FAIL_THRESHOLD;

int working_counter_fail_count = 0;


int ERROR_COUNT_THRESHOLD;

int error_count = 0;


int MOVE_FAIL_THRESHOLD;


int g_num_of_slaves;


pthread_t ECAT2_tid;

char main_if_name[MAX_IFNAME] = {0};


// Home sensor position from front, anti-clockwise. 400000 is one rotation

int *arr_home_sensor_position = NULL;


// Homing start offset

int *arr_start_offset = NULL;


servo_rxpdo_t **motor_rxpdos = NULL;

servo_txpdo_t **motor_txpdos = NULL;


int main(int argc, char *argv[])

{

   printf("SOEM (Simple Open EtherCAT Master)\nServo controller\n");




   int config_result = InitRuntimeFrom("config.json");


   if (config_result < 0){


      printf("failed to init with config json\n");

      return config_result;

   }


   signal(SIGINT, sigint_handler);



   if (argc > 1)

   {
      ec_adaptert *adapter = NULL;

      printf("Usage: controller ifname1\nifname = eth0 for example\n");


      printf("\nAvailable adapters:\n");

      adapter = ec_find_adapters();

      while (adapter != NULL)

      {

         printf("    - %s  (%s)\n", adapter->name, adapter->desc);

         adapter = adapter->next;

      }

      ec_free_adapters(adapter);


   }

   else

   {

      osal_thread_create(&thread1, 128000, &ecatcheck, NULL);

      /* start cyclic part */

      ECAT2_lifecycle(main_if_name);


   }


   printf("End program\n");

   return (0);

}


