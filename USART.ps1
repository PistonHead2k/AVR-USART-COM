#USART RS232 powershell script by Mateo Proruk

#Input variables: .\USART.ps1 -COM xxx .etc
param
(
    [string]$Mode = "NULL",
    [string]$Port = "NULL",
    [string]$Baud = "NULL",
    [string]$Parity = "NULL",
    [string]$Data = "NULL",
    [string]$Stop = "NULL"
)

Write-Host "USART RS232 powershell script by Mateo Proruk"


if (!$Mode)
{
    Write-Host "Please Specify Mode (-Mode Send, Receive)"
    exit
}

if (!$Port)
{
    Write-Host "Please Specify COM Port (-Port COM1, COM2, etc)"
    exit
}

if (!$Baud)
{
    Write-Host "Please Specify Baud Rate (-Baud 9600, 115200, etc)"
    exit
}

switch ($Parity)
{
    "Even" {}
    "Odd" {}
    "None" {}
    "Mark" {}
    "Space" {}
    default 
    {
        Write-Host "Please Specify Parity Bits (-Parity Even, Odd, None, Mark, Space)"
        exit
    }
}

switch ($Data)
{
    "4" {}
    "5" {}
    "6" {}
    "7" {}
    "8" {}
    default 
    {
        Write-Host "Please Specify Data Bits (-Data 4, 5, 6, 7, 8)"
        exit
    }
}

switch ($Stop)
{
    "0" {$Stop = [System.IO.Ports.StopBits]::None}
    "1" {$Stop = [System.IO.Ports.StopBits]::One}
    "1.5" {$Stop = [System.IO.Ports.StopBits]::OnePointFive}
    "2" {$Stop = [System.IO.Ports.StopBits]::Two}
    default 
    {
        Write-Host "Please Specify Stop Bits (-Stop 0, 1, 1.5, 2)"
        exit
    }
}

$ioport = New-Object System.IO.Ports.SerialPort $Port, $Baud, $Parity, $Data, $Stop
$ioport.Open()

# To read data continuously (example)
while($true) 
{
    try 
    {
        $data = $ioport.ReadLine()
        Write-Host $data
    } 
    catch [System.TimeoutException] 
    {
        # Handle timeout if no data is received
        Write-Host "System.TimeoutException"
    }
    
    if ([console]::KeyAvailable)
    {
        $key = [System.Console]::ReadKey($true) #$true means the key pressed wont appear on console before Send:
        if ($key.Key -eq "Escape")
        {exit}

        if ($Mode -eq "Send")
        {
            $ShellLine = Read-Host "Send"
            $ioport.WriteLine($ShellLine)
        }
        else
        {exit}
    }
}