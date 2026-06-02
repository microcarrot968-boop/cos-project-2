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

  int tmp, avg_power, avg_temp, avg_humid, month;
  int sum_power;
  time_t ts;
  struct tm *tm;

  tdata = ds->getTemperatureData();
  hdata = ds->getHumidityData();
  num = ds->getNumHouseData();

  // Feature vector:
  // [avg_power, avg_temperature, avg_humidity, month]
  // Target: next-day average power
  // dimension = 4, index = 0

  // Get average daily temperature
  avg_temp = (int)tdata->getValue();

  // Get average daily humidity
  avg_humid = (int)hdata->getValue();

  // Get average power value from all house data
  sum_power = 0;
  for (int i = 0; i < num; i++)
  {
    house = ds->getHouseData(i);
    pdata = house->getPowerData();
    tmp = (int)pdata->getValue();

    sum_power += tmp;
  }

  avg_power = sum_power / num;

  // Get month value from the timestamp
  ts = ds->getTimestamp();
  tm = localtime(&ts);
  month = tm->tm_mon + 1;

  // Initialize memory to send to the network manager
  memset(ret, 0, BUFLEN);
  *dlen = 0;
  p = ret;

  // Save values in memory in this order:
  // avg_power: 2 bytes
  // avg_temp: 1 byte
  // avg_humid: 1 byte
  // month: 1 byte
  VAR_TO_MEM_2BYTES_BIG_ENDIAN(avg_power, p);
  *dlen += 2;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(avg_temp, p);
  *dlen += 1;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(avg_humid, p);
  *dlen += 1;

  VAR_TO_MEM_1BYTE_BIG_ENDIAN(month, p);
  *dlen += 1;

  return ret;
}
