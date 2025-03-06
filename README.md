

![redis](https://img.shields.io/badge/GPU-Windows-red)

# PrismGPU 
目前暂计划使用qemu模拟物理设备进行驱动开发，以及进行架构和验证框架的摸索。成果目前可以使用qemu加载支持VBE仅支持输出功能的设备模拟器，
当前成果展示在最下方。

## Build

### 下载qemu并创建模拟环境
1. 在qemu网站下载稳定版qemu，本设备模拟使用qemu9.0.0

2. 在下载完成之后to/Simulator/prism.c文件移动到qemu/hw/display/文件夹下

3. ```meson
   在qemu/hw/display/meson.build文件中添加如下代码：
   softmmu_ss.add(when: 'CONFIG_PRISM', if_true: files('prism.c'))
   ```

4. ```Kconfig
   在qemu/hw/display/Kconfig文件中添加如下代码：
    config PRISM
       bool
       default y if PCI_DEVICES
       depends on PCI
   ```

   

5. ```bash
   cd qemu
   mkdir build
   make -j8
   sudo make install
   ```

6. 下载Windows镜像本项目使用的Windows 10 专业版。

7. ```bash
   创建Windows.img
   qemu-img create -f qcow2 win10.img 40G #40g为自主选定
   ```

8. ```bash
   cd ../..
   mkdir prism
   cd prism
   touch start.sh
   echo '#!/bin/bash' > start.sh
   echo 'qemu-system-x86_64\
   -hda ./win10.img\
   -cdrom win10.iso\ #首次启动时增加，后续可以删除
   -boot d -enable-kvm -machine q35\
   -device intel-iommu -smp 4,sockets=1,cores=2 -m 4G\
   -device prism\
   -net nic,model=e1000 -net user -usbdevice tablet' >> start.sh
   ```

### 编译驱动程序并安装驱动

1. 编译驱动程序，首先安装WDK和SDK具体参考MSDN安装流程并安装VS 2022或者其他版本，本项目使用2022版。在一切准备就绪之后将to/software/KMD中的KMDOD使用vs打开之后生成驱动程序SimpleDisplay文件就是驱动。

2. 打开qemu模拟器中的windows进入之后以管理员身份运行CMD

3. ```dos
   在CMD运行如下命令打开测试模式（避免缺少数字签名禁止驱动安装）
   bcdedit /debug on
   bcdedit /set testsigning on
   bcdedit /set nointegritychecks on
   ```

4. 将SimpleDisplay复制到qemu虚拟机打开文件夹选择.inf安装信息文件右键安装即可。

5. 完毕。

## 调试

### 驱动和模拟器调试

暂时搁置，驱动调试Windbg模拟器GDB。

## 成果

![now](Pic/设备.PNG)
![now](Pic/驱动.PNG)
![now](Pic/资源.PNG)