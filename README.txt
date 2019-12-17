/*********************************************************************************************************
     THIS IS A SAMPLE MOSQUITTO PROJECT TO PUBLISH/SUBSCRIBE BLE,GNSS DATA
   
     OWNER :RIZWAN
    
***********************************************************************************************************/
Dependencies:
============

- In order to build the code we need Yocto SDK
- To install the SDK run the following command: source <sdk_name>.sh  
- For the cross compilation source the file located inside sysroot

 to expose the environment variable
- Run make to generate the binary
- Run the following command to execute the binary
  
   qemu-<arch> -L <sysroot_path>/<machine_name> <binary_name>

- Install the following binaries mosquitto_pub,mosquitto_sub by running the following command
 
    sudo apt-get install mosquitto_clients

Procedure:
=========

- Run the binary from above step   

- Follow the below steps for testing BLE and GNSS Data

**********************BLE DATA**************************************************************************************************

- Open 2 terminals and in one terminal type the following command

  mosquitto_sub -t app/data/ble
  
  This command is to subscribe ble data
  
  In the another terminal type this command 

  mosquitto_pub -t app/server/ble -m data,deviceId=n2_190225_0038,wheelbasedvehiclespeed=529.219,vin=na,enginespeed=1086,enginehours=2973.15,mileagetrue=67019415,enginemove=0,enginestatus=0,gearposition=1


**********************GNSS DATA****************************************************************************************************

- Open 2 terminals and in one terminal type the following command

  mosquitto_sub -t app/data/gnss
  
  This command is to subscribe ble data
  
  In the another terminal type this command

 mosquitto_pub  -t app/server/gnss -m data,Hdop=0,GnssLockStatus=2,NumSats=0,GnssSpeed=0.000000,Altitude=0.000000,Heading=0,Longitude=0.000000,Latitude=0.000000



