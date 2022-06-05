#!/bin/sh -e
#
# Build the catch2 tarball

echo "------------ working directory: `pwd`"

OUTPUT_PATH="$1"
SNAPCATCH2_VERSION="$2"
SCRIPT_DIR="`dirname $0`"
SOURCE_DIR="`dirname ${SCRIPT_DIR}`"

echo "--- output directory: ${OUTPUT_PATH}"
echo "--- source directory: ${SOURCE_DIR}"
echo "--- Catch2 version: ${SNAPCATCH2_VERSION}"

rm -rf "${OUTPUT_PATH}"

tar xf ${SOURCE_DIR}/catch2-${SNAPCATCH2_VERSION}.tar.gz

# Apply patch if present
if test -f ${SOURCE_DIR}/Catch2-${SNAPCATCH2_VERSION}.patch
then
	echo "--- apply patch Catch2-${SNAPCATCH2_VERSION}.patch"

	# TODO: I don't think this is 100% correct, the exact filename should
	#       be part of the .patch file
	#
	patch ${OUTPUT_PATH}/projects/CMakeLists.txt <${SOURCE_DIR}/Catch2-${SNAPCATCH2_VERSION}.patch
else
	echo "--- no patch"
fi

mkdir -p ${OUTPUT_PATH}/out
mkdir -p ${OUTPUT_PATH}/projects/Generated
(
	cd ${OUTPUT_PATH}/projects/Generated

	echo "--- generate Makefiles"
	cmake -DCATCH_INSTALL_EXTRAS=on \
		-DCATCH_INSTALL_DOCS=on \
		-DCATCH_DEVELOPMENT_BUILD=off \
		-DBUILD_TESTING=off \
		-DCMAKE_INSTALL_PREFIX=${OUTPUT_PATH}/out \
		-DCMAKE_POSITION_INDEPENDENT_CODE=on \
			../..
)

echo "--- build"
VERBOSE=1 make -C ${OUTPUT_PATH}/projects/Generated

echo "--- install"
VERBOSE=1 make -C ${OUTPUT_PATH}/projects/Generated install


# In 16.04 the cmake directory we had to install was "hidden". This is fixed
# in newer version so we do not do anything about that at the moment.
#
#if test -d ${OUTPUT_PATH}/out/lib/*-linux-gnu/cmake
#then
#	cp -r ${OUTPUT_PATH}/out/lib/*-linux-gnu/cmake ${OUTPUT_PATH}/out/share/.
#fi
#

