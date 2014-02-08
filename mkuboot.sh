#!/bin/sh
	
export BUILD_DIR="tmp/build"
export CROSS_COMPILE=arm-hisiv100nptl-linux-
make distclean
make hi3518a_config
make
make env

echo "--------------------------------------------------------------------------"

echo "cp $BUILD_DIR/u-boot.bin ./uboot_tools"
cp $BUILD_DIR/u-boot.bin ./uboot_tools

export UBOOT_REG_BIN=reg_info_hi3518a.bin
export UBOOT=u-boot-200MHZ.bin
pushd uboot_tools
./mkboot.sh $UBOOT_REG_BIN $UBOOT
popd
echo "cp uboot_tools/$UBOOT $tftp_dir"
cp uboot_tools/$UBOOT $tftp_dir

