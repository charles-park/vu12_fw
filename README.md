# vu12_fw
저가의 ch55x chip을 사용한 LVDS2HDMI LCD Monitor control(VU12, 1920x720)

ch55x 참조 사이트
* <https://github.com/DeqingSun/ch55xduino>
* <https://cdn.jsdelivr.net/gh/DeqingSun/ch55xduino@playground>
* <https://github.com/topics/ch552>
* vnproch55x <https://github.com/LoveMHz/vnproch55x/tree/master>
  
arduino-cli install 참조사이트(X86)
* <https://m.blog.naver.com/roboholic84/221815428642>

Video Timings Calculator
* https://tomverbeure.github.io/video_timings_calculator

EDID Read & Check
* https://wiki.odroid.com/accessory/display/vu_series/vu8/update_edid

Custom EDID Test
* https://wiki.odroid.com/common/hdmi_howto/custombuilt?s[]=custom&s[]=hdmi

## Install Package 
* arduino-cli install
```
  root@odroid:~# mkdir cli
  root@odroid:~/cli# cd cli
  root@odroid:~/cli# curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
  root@odroid:~/cli# cp ./bin/arduino-cli /bin/
```

* config파일 생성 및 보드 설치
```
  root@odroid:~/cli# arduino-cli config init
  root@odroid:~/cli# vi ~/.arduino15/arduino-cli.yaml
```

* board package urls 추가
```
  additional_urls:
    - https://raw.githubusercontent.com/DeqingSun/ch55xduino/ch55xduino/package_ch55xduino_mcs51_index.json
```

* board package index file download (ARM Board에 설치시에만 필요함. add url을 참조하여 진행하지 못하는 현상이 발생)
```
  root@odroid:~/cli# cd ~/.arduino15
  root@odroid:~/.arduino15# wget https://raw.githubusercontent.com/DeqingSun/ch55xduino/ch55xduino/package_ch55xduino_mcs51_index.json
```

* board package & core install
```
  root@odroid:~/.arduino15# arduino-cli core install CH55xDuino:mcs51
```
 
## Compile & Download
```
  # clone vu12_fw repo
  root@odroid:~# git clone https://github.com/charles-park/vu12_fw
  # compile only
  root@odroid:~# arduino-cli compile -b CH55xDuino:mcs51:ch552 vu12_fw
  # compile & download
  root@odroid:~# arduino-cli compile -b CH55xDuino:mcs51:ch552 vu12_fw -u -p /dev/ttyACM0
  # download only
  root@odroid:~# arduino-cli upload -b CH55xDuino:mcs51:ch552 -i vu12_fw.hex -p /dev/ttyACM0

```
## ch55x 전용 tool을 사용한 download (script/download.sh 참조)
* bootloader rule 복사
```
  root@odroid:~/cli# wget https://cdn.jsdelivr.net/gh/DeqingSun/ch55xduino@playground/ch55xduino/tools/linux_arm/99-ch55xbl.rules
  root@odroid:~/cli# cp 99-ch55xbl.rules /etc/udev/rules.d/
```
* 전용 tool download
```
  root@odroid:~/cli# wget https://cdn.jsdelivr.net/gh/DeqingSun/ch55xduino@playground/ch55xduino/tools/linux_arm/vnproch55x
  root@odroid:~/cli# chmod 777 ./vnproch55x
  root@odroid:~/cli# cp vnproch55x /bin/
```
* Download (Nomal state : /dev/ttyACM0 node가 있고 1209:c550 device가 있는 경우)
```
  # UART(/dev/ttyACM0)의 baud를 변경하여 bootloader모드로 진입 후 다운로드 함.
  usbreset 1209:c550
  stty -F /dev/ttyACM0 1200
  vnproch55x -r 2 -t ch552 <fw_file.hex>
  stty -F /dev.ttyACM0 9600
```
* Donwload (Boot state : /dev/ttyACM0 node가 없고 4348:55e0 device가 있는 경우)
```
  usbreset 43489:55e0
  vnproch55x -r 2 -t ch552 <fw_file.hex>HOST -> UPS (4 Bytes)									
```

 **주의 : Firmware Download후 반드시 "stty -F /dev/ttyACM0 9600" 실행. 실행 없는 경우 UART접근시 reset되어짐.**

## Hex file 생성위치
```
  /tmp/arduino/sketches/***/vu12_fw.hex
```
