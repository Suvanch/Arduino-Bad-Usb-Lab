$portObj = [System.IO.Ports.SerialPort]::new('COM8',115200)
$portObj.Open()
$listing = $portObj.ReadExisting()   # read everything currently in buffer
$portObj.Close()

# Send it straight to Notepad:
$temp = [IO.Path]::Combine($env:TEMP,'arduino_dir.txt')
$listing | Out-File -Encoding ascii $temp
Start-Process notepad.exe $temp
