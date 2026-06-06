#include "process_manager.h"
#include "opcode.h"
#include "byte_op.h"
#include "setting.h"
#include <cstring>
#include <iostream>
#include <ctime>
using namespace std;

ProcessManager::ProcessManager()
{
  this->num = 0;
}

void ProcessManager::init()
{
}

// TODO: You should implement this function if you want to change the result of the aggregation
uint8_t *ProcessManager::processData(DataSet *ds, int *dlen)
{
  uint8_t *ret, *p;
  int num;
  HouseData *house;
  TemperatureData *tdata;
  HumidityData *hdata;
  PowerData *pdata;

  ret = (uint8_t *)malloc(BUFLEN);

  int tmp;
  int avg_power;
  int avg_temp;
  int avg_humid;
  int max_temp;
  int min_temp;
  int temp_range;
  int month;
  int sum_power;

  time_t ts;
  struct tm *tm;

  // Get daily temperature and humidity data from the dataset.
  tdata = ds->getTemperatureData();
  hdata = ds->getHumidityData();

  // Get the number of houses in this daily dataset.
  num = ds->getNumHouseData();

  // Calculate the average power value across all houses.
  // Each HouseData object has one power value for this day.
  sum_power = 0;
  for (int i = 0; i < num; i++)
  {
    house = ds->getHouseData(i);
    pdata = house->getPowerData();
    tmp = (int)pdata->getValue();

    sum_power += tmp;
  }

  avg_power = sum_power / num;

  // Get common temperature and humidity values.
  // getValue() returns the daily average value.
  avg_temp = (int)tdata->getValue();
  avg_humid = (int)hdata->getValue();

  // Get daily maximum and minimum temperature.
  max_temp = (int)tdata->getMax();
  min_temp = (int)tdata->getMin();

  // Calculate temperature range from raw temperature data.
  temp_range = max_temp - min_temp;

  // Get the month value from the timestamp.
  // tm_mon starts from 0, so we add 1 to make January = 1.
  ts = ds->getTimestamp();
  tm = localtime(&ts);
  month = tm->tm_mon + 1;

  // Initialize the memory buffer to send to the network manager.
  memset(ret, 0, BUFLEN);
  *dlen = 0;
  p = ret;

  // ============================================================
  // FEATURE VERSION 1
  // Feature vector:
  // [avg_power, avg_temp, avg_humid, month]
  //
  // dimension = 4
  // target index = 0
  //
  // byte layout:
  // avg_power : 2 bytes
  // avg_temp  : 1 byte
  // avg_humid : 1 byte
  // month     : 1 byte
  // total     : 5 bytes
  //
  // Server should parse:
  // buf[0:2] = avg_power
  // buf[2:3] = avg_temp
  // buf[3:4] = avg_humid
  // buf[4:5] = month
  // ============================================================

/*
  VAR_TO_MEM_2BYTES_BIG_ENDIAN(avg_power, p);
  *dlen += 2;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(avg_temp, p);
  *dlen += 1;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(avg_humid, p);
  *dlen += 1;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(month, p);
  *dlen += 1;
*/


  // ============================================================
  // FEATURE VERSION 2
  // Feature vector:
  // [avg_power, avg_temp, temp_range, avg_humid, month]
  //
  // dimension = 5
  // target index = 0
  //
  // byte layout:
  // avg_power  : 2 bytes
  // avg_temp   : 1 byte
  // temp_range : 1 byte
  // avg_humid  : 1 byte
  // month      : 1 byte
  // total      : 6 bytes
  //
  // Server should parse:
  // buf[0:2] = avg_power
  // buf[2:3] = avg_temp
  // buf[3:4] = temp_range
  // buf[4:5] = avg_humid
  // buf[5:6] = month
  // ============================================================
/*
  VAR_TO_MEM_2BYTES_BIG_ENDIAN(avg_power, p);
  *dlen += 2;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(avg_temp, p);
  *dlen += 1;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(temp_range, p);
  *dlen += 1;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(avg_humid, p);
  *dlen += 1;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(month, p);
  *dlen += 1;
*/


  // ============================================================
  // FEATURE VERSION 3
  // Feature vector:
  // [avg_power, max_temp, min_temp, avg_humid, month]
  //
  // dimension = 5
  // target index = 0
  //
  // byte layout:
  // avg_power : 2 bytes
  // max_temp  : 1 byte
  // min_temp  : 1 byte
  // avg_humid : 1 byte
  // month     : 1 byte
  // total     : 6 bytes
  //
  // Server should parse:
  // buf[0:2] = avg_power
  // buf[2:3] = max_temp
  // buf[3:4] = min_temp
  // buf[4:5] = avg_humid
  // buf[5:6] = month
  // ============================================================

  VAR_TO_MEM_2BYTES_BIG_ENDIAN(avg_power, p);
  *dlen += 2;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(max_temp, p);
  *dlen += 1;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(min_temp, p);
  *dlen += 1;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(avg_humid, p);
  *dlen += 1;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(month, p);
  *dlen += 1;
  

  return ret;
}