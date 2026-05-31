using System;
using System.IO.Ports;
using System.Threading;

class Program
{
    static SerialPort? _serialPort;

    static void Main(string[] args)
    {
        Console.WriteLine("=== UNITY BUILD WATCHER ===");

        // 1. Liệt kê cổng
        string[] ports = SerialPort.GetPortNames();
        if (ports.Length == 0)
        {
            Console.WriteLine("No COM port found. Please check the cable.!");
            return;
        }

        for (int i = 0; i < ports.Length; i++)
            Console.WriteLine($"{i}: {ports[i]}");

        Console.Write("Select the COM port number: ");
        int choice = int.Parse(Console.ReadLine() ?? "0");

        // 2. Mở kết nối
        try
        {
            _serialPort = new SerialPort(ports[choice], 9600);
            _serialPort.Open();
            Console.WriteLine($"Connected to {ports[choice]}");
        }
        catch (Exception e)
        {
            Console.WriteLine($"Error: {e.Message}");
            return;
        }

        // 3. Theo dõi log
        string logPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), @"Unity\Editor\Editor.log");
        Console.WriteLine($"Connected to: {logPath}");

        using (var fs = new FileStream(logPath, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
        using (var reader = new StreamReader(fs))
        {
            fs.Seek(0, SeekOrigin.End);
            
            byte[] buffer = new byte[4096];

            while (true)
            {
                if (fs.Position < fs.Length)
                {
                    int bytesRead = fs.Read(buffer, 0, buffer.Length);

                    string text = System.Text.Encoding.UTF8.GetString(buffer, 0, bytesRead);

                    // if (text.Contains("Build"))
                    //     Console.Write(text);

                    if (text.Contains("Build completed") && text.Contains("Succeeded"))
                    {
                        Console.WriteLine(">> Build successful! Submit 'success'");
                        _serialPort.WriteLine("success");
                    }

                    if (text.Contains("Build completed") && text.Contains("Failed"))
                    {
                        Console.WriteLine(">> Build failed! Submit 'failed'");
                        _serialPort.WriteLine("failed");
                    }
                }

                Thread.Sleep(10);
            }
        }
    }
}