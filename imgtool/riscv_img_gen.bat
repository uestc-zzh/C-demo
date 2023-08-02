@echo off

if "%8"=="" (
    echo "Help: riscv_img_gen.bat version input.out output_img_name"
    echo "sign_key, encrypt_key, output_secure_img_name"
    echo "image_type(bootloader or firmware)"
    echo "image_out_path (subsys_image_path)"
    goto :done
)

if not exist "%2" (
    echo Can't find "%2" file & goto :done
)

set version=%1
set input=%2
set img_out_name=%3
set image_temp_dir=%3
set encrypt_key=%4
set sign_key=%5
set signed_img_out_name=%6
set img_type=%7
set image_out_path=%8
set subsys_image_path=%9
set /a page_size=256

cd ../../imgtool
if exist %image_temp_dir% (
    rmdir /q /s %image_temp_dir%
)
md %image_temp_dir%
cd %image_temp_dir%

if "%img_type%"=="bootloader" (
    set img_is_bl="True"
) else if "%img_type%"=="firmware" (
    set img_is_bl="False"
) else (
    echo "Invalid type of image to generate!" & goto :done
)

riscv-nuclei-elf-objcopy -j .flash -O binary %input% flash
for /f "tokens=5" %%i in ('riscv-nuclei-elf-objdump -h %input%  ^| findstr "\.flash"') do (
    set flash_load_addr=0x%%i
)
if "%img_type%"=="bootloader" (
    call :create_part flash %flash_load_addr% %img_is_bl% "True" "False"
) else (
    call :create_part flash %flash_load_addr% %img_is_bl% "False" "False"
)

riscv-nuclei-elf-objcopy -j .siram -O binary %input% siram
for /f "tokens=5" %%i in ('riscv-nuclei-elf-objdump -h %input%  ^| findstr "\.siram"') do (
    set siram_load_addr=0x%%i
)
if "%img_type%"=="bootloader" (
    call :create_part siram %siram_load_addr% %img_is_bl% "False" "True"
) else (
    call :create_part siram %siram_load_addr% %img_is_bl% "False" "True"
)

riscv-nuclei-elf-objcopy -j .ilm -O binary %input% ilm
for /f "tokens=5" %%i in ('riscv-nuclei-elf-objdump -h %input%  ^| findstr "\.ilm"') do (
    set ilm_load_addr=0x%%i
)
:: it doesn't contain entry point and need to load to RAM
call :create_part ilm %ilm_load_addr% %img_is_bl% "True" "True"

riscv-nuclei-elf-objcopy -j .sdram -O binary %input% sdram
for /f "tokens=5" %%i in ('riscv-nuclei-elf-objdump -h %input%  ^| findstr "\.sdram"') do (
    set load_addr=0x%%i
)
:: it doesn't contain entry point and need to load to RAM
call :create_part sdram %load_addr% %img_is_bl% "False" "True"

:: generate clean image
if "%img_type%"=="bootloader" (
    type flash.img > %img_out_name%.img
    for /l %%i in (1,1,20000) do echo %%i>nul
    type siram.img >> %img_out_name%.img
    for /l %%i in (1,1,20000) do echo %%i>nul
    type ilm.img >> %img_out_name%.img
    for /l %%i in (1,1,20000) do echo %%i>nul
    type sdram.img >> %img_out_name%.img
    for /l %%i in (1,1,20000) do echo %%i>nul
) else (
    type ilm.img > %img_out_name%.img
    for /l %%i in (1,1,20000) do echo %%i>nul
    type siram.img >> %img_out_name%.img
    for /l %%i in (1,1,20000) do echo %%i>nul
    type sdram.img >> %img_out_name%.img
    for /l %%i in (1,1,20000) do echo %%i>nul
    type flash.img >> %img_out_name%.img
    for /l %%i in (1,1,20000) do echo %%i>nul
)

call :export_img_file %img_out_name%.img
echo "Finished generate target: %img_out_name%.img, version is %version%"

:: generate clean + subsystem image
if NOT "%subsys_image_path%"=="" (
    type %img_out_name%.img > %img_out_name%_combine.img
    :: append subsystem image
    for %%j in (%subsys_image_path%\*.img) do (
        echo "Clean image append %%j"
        type %%j >> %img_out_name%_combine.img
        for /l %%i in (1,1,20000) do echo %%i>nul
    )
    call :export_img_file %img_out_name%_combine.img
    echo "Finished generate target: %img_out_name%_combine.img, version is %version%"
)

:: generate secure image
if NOT "%sign_key%" == """" (
    if "%img_type%"=="bootloader" (
        call :create_secure_part %img_out_name%.img %flash_load_addr% ^
                                 %sign_key% %signed_img_out_name% %img_is_bl%
    ) else (
        call :create_secure_part %img_out_name%.img %ilm_load_addr% ^
                                 %sign_key% %signed_img_out_name% %img_is_bl%
    )
    call :export_img_file %signed_img_out_name%.img
    echo "Finished generate target: %signed_img_out_name%.img, version is %version%"

    :: generate secure + subsystem image
    if NOT "%subsys_image_path%"=="" (
        type %signed_img_out_name%.img > %signed_img_out_name%_combine.img
        :: append subsystem image
        for %%j in (%subsys_image_path%\*.img) do (
            echo "Secure image append %%j"
            type %%j >> %signed_img_out_name%_combine.img
            for /l %%i in (1,1,20000) do echo %%i>nul
        )
    )
    call :export_img_file %signed_img_out_name%_combine.img
    echo "Finished generate target: %signed_img_out_name%_combine.img, version is %version%"
)

cd ..
goto :done


:create_part
    :: calculate size of img
    for /f "tokens=3" %%i in ('dir /-c %1 ^| find "%1"') do (
        set /a size=%%i
    )
    if %size% EQU 0 (
        echo "No %1 generated."
        goto :done
    )

    :: total flash pages of img
    set /a page=%size%/%page_size%
    set /a rem=%size%%%%page_size%
    if %rem% GTR 0 (
        set /a page=%page%+1
    )

    :: add header and trailer
    set /a page=%page%+2
    set /a slot_size=%page%*%page_size%
    echo "part: %1 payload size: %size% page num: %page% slot size: %slot_size% ld_addr: %2"

    :: generate basic clean image
    start /wait ../imgtool.exe create --align 4 ^
            --version %version% ^
            --overwrite-only ^
            -H %page_size% --pad-header ^
            -L %2 ^
            --slot-size %slot_size% --pad ^
            --bl-img %3 ^
            --entry %4 ^
            --ram-load %5 ^
            %1 %1.img
    if %errorlevel% NEQ 0 (
        echo "Generate clean %1.img by imgtool failed!"
        exit
    )
    :: return
    goto :done

:create_secure_part
    :: calculate size of img
    for /f "tokens=3" %%i in ('dir /-c %1 ^| find "%1"') do (
        set /a size=%%i
    )
    if %size% EQU 0 (
        echo "No %1 generated."
        goto :done
    )

    :: total flash pages of img
    set /a page=%size%/%page_size%
    set /a rem=%size%%%%page_size%
    if %rem% GTR 0 (
        set /a page=%page%+1
    )

    :: add header and trailer
    set /a page=%page%+2
    set /a slot_size=%page%*%page_size%
    echo "part: %1 payload size: %size% page num: %page% slot size: %slot_size% ld_addr: %2 key: %3"

    :: generate secure image(only signature)
    start /wait ../imgtool.exe create --align 4 ^
            --version %version% ^
            --overwrite-only ^
            -H %page_size% --pad-header ^
            --slot-size %slot_size% --pad ^
            -L %2 ^
            -k %3 ^
            --bl-img %5 ^
            %1 %4.img
    if %errorlevel% NEQ 0 (
        echo "Generate secure %1.img by imgtool failed!"
        exit
    )
    :: return
    goto :done

:export_img_file
    if exist "%image_out_path%\%1" (
        del /s /q /f %image_out_path%\%1
    )
    copy %1 %image_out_path%
    :: return
    goto :done

:done
