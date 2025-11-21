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


switch ($Mode)
{ 
    "SendByte" {}
    "SendChar" {}
    "SendString" {}
    "Receive" {}
    default 
    {
    Write-Host "Please Specify Mode (-Mode SendByte, SendChar, SendString, Receive)"
    exit
    }
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
        if ($key.Key -eq "Escape" -or $Mode -eq "Receive")
        {
            $ioport.Close()
            exit
        }

        #Halts Shell Input and stores the string in $ShellLine
        $ShellLine = Read-Host "Send"

        #Sends The Whole String as Is
        if ($Mode -eq "SendString")
        {
            $ioport.WriteLine($ShellLine)
        }

        #Sends One Character at a time
        if ($Mode -eq "SendChar")
        {   
            #Removes The Line Feed/Null Termination Char
            for ($i = 0 ; $i -lt $ShellLine.length; $i++)
            {
                $ioport.Write($ShellLine[$i])
            }
            
        }

        if ($Mode -eq "SendByte")
        {     
            $byteValue = $null   # Variable to store the converted byte
            $try = [Byte]::TryParse($ShellLine, [ref]$byteValue)

            if ($try)
            {
                $ioport.Encoding = [System.Text.Encoding]::GetEncoding(1252)
                $ioport.Write([char]$byteValue)
                Write-Host Please Write a Byte
            }
            else 
            {
                Write-Host Please Send 0 - 255 Decimal Value 
            }  
        }
    }
}