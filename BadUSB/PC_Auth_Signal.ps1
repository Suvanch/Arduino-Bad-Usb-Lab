# ---- arduino_handshake.ps1 ----
$UsbVid = '2341'          # HoodLoader2 VID
$UsbPid = '484C'          # HoodLoader2 PID
$Baud   = 9600

Write-Host '[INFO] Waiting for HoodLoader2 CDC port ...'
do {
    Start-Sleep -Milliseconds 300
    $port = Get-CimInstance Win32_PnPEntity |
            Where-Object { $_.PNPDeviceID -like '*VID_2341&PID_484C*' } |
            Where-Object { $_.Name -match 'COM(\d+)' } |
            ForEach-Object {
                $_.Name -match 'COM(\d+)' | Out-Null
                'COM' + $Matches[1]
            }
} until ($port)

Write-Host '[INFO] Found' $port '- opening at' $Baud 'bps.'
$sp = [System.IO.Ports.SerialPort]::new($port,$Baud)
$sp.Open()
$sp.Write('SIVAN-PC')
$sp.Close()
Write-Host '[INFO] Handshake sent and port closed.'
# ---------------------------------