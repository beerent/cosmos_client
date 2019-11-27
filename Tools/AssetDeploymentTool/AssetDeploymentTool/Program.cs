using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.IO;

namespace AssetDeploymentTool
{
    class Program
    {
        static void Main(string[] args)
        {
            string source = "";
            string destination = "";
            List<string> errors = new List<string>();            

            Console.WriteLine("Number of command line parameters = {0}", args.Length);
            for (int i = 0; i < args.Length; i++)
            {
                Console.WriteLine("Arg[{0}] = [{1}]", i, args[i]);
                string[] tokens = args[i].Split('=');
                if(tokens.Count() == 2)
                {
                    switch (tokens[0].Trim().ToLower())
                    {
                        case "src":
                            source = tokens[1];
                            break;
                        case "dest":
                            destination = tokens[1];
                            break;
                    }
                }
            }

            string[] assetPaths =
            {
                @"Archetypes\AI\AIGoal",
                @"Archetypes\AI\NPCAI",
                @"Archetypes\AI\AIBehavior",
                @"CollisionData",
                @"CollisionData\BasicCollisionTypes",
                @"CollisionData\CustomCollisionTypes",
                @"CollisionData\CollisionGroups",
                @"Textures",
                @"UIComponentArchetypes",
                @"UIRenderables",
                @"Renderables",
                @"Renderables\CollisionRenderables",
                @"Meshes",
                @"FontArchetypes",
                @"EntityGroupData",
                @"EntityArchetypes",
                @"Materials",
                @"Shaders",
                @"Shaders\Fragment",
                @"Shaders\Programs",
                @"Shaders\Vertex"
            };
            
            if (source != "" && destination != "")
            {
                foreach( string assetPath in assetPaths)
                {
                    string subSource = source + "\\" + assetPath;
                    deploy(subSource, destination, ref errors);
                }
            }

            Console.WriteLine("DONE!");

            if(errors.Count() > 0)
            {

                Console.WriteLine("==================================================");
                foreach(string error in errors)
                {
                    Console.WriteLine(error);
                }
                Console.WriteLine("Errors detected: {0}" , errors.Count());
                Console.WriteLine("==================================================");
                Thread.Sleep(1000000);
            }
            else {
                Thread.Sleep(1000);
            }
        }

        static string getDestinationFileName(string fileName)
        {
            string extension = Path.GetExtension(fileName);
            switch(extension.ToLower())
            {
                case ".png":
                    return Path.GetFileNameWithoutExtension(fileName) + ".trb";
                default:
                    return fileName.ToLower();
            }
        }

        static void performTransformationOnFile(string sourcePath, string targetPath, ref List<string> errors)
        {
            string extension = Path.GetExtension(sourcePath);
            switch (extension.ToLower())
            {
                case ".png":
                    if(Path.GetFileName(sourcePath).ElementAt(0) != '%')
                    {
                        convertPNGToFrameBuffer(sourcePath, targetPath, ref errors);
                    }
                    break;
                default:
                    copyFile(sourcePath, targetPath, ref errors);
                    break;
            }
        }

        static void copyFile(string sourcePath, string targetPath, ref List<string> errors)
        {
            try
            {
                Console.WriteLine("Copying file {0}", sourcePath);
                File.Copy(sourcePath, targetPath, true);
                File.SetAttributes(targetPath, FileAttributes.Normal);
            }
            catch (Exception)
            {
                string error = string.Format("Failed to copy file {0}", sourcePath);
                Console.WriteLine(error);
                errors.Add(error);
            }
        }

        static void convertPNGToFrameBuffer(string sourcePath, string targetPath, ref List<string> errors)
        {

            try
            {
                Console.WriteLine("Converting From PNG to FrameBuffer file {0}", sourcePath);
                System.Diagnostics.Process pProcess = new System.Diagnostics.Process();
                pProcess.StartInfo.FileName = @"TextureConverter.exe";
                pProcess.StartInfo.Arguments = "mode=SINGLE_PNG_TO_BUFFER path=\"" + sourcePath + "\" outPath=\"" + targetPath; //argument
                pProcess.StartInfo.UseShellExecute = false;
                pProcess.StartInfo.RedirectStandardOutput = true;
                pProcess.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                pProcess.StartInfo.CreateNoWindow = true; //not diplay a windows
                pProcess.Start();
                string output = pProcess.StandardOutput.ReadToEnd(); //The output result
                pProcess.WaitForExit();
                Console.WriteLine(output);
            }
            catch (Exception)
            {
                string error = string.Format("Failed to convert file {0}", sourcePath);
                Console.WriteLine(error);
                errors.Add(error);
            }

        }

        static void deploy(string source, string destination, ref List<string> errors)
        {
            Console.WriteLine("Source {0} Destination {1}", source, destination);
            try
            {
                if (!System.IO.Directory.Exists(destination))
                {
                    Directory.CreateDirectory(destination);
                }
            }
            catch( Exception)
            {
                string error = string.Format("Failed to create directory ");
                Console.WriteLine(error);
                errors.Add(error);
            }

            string[] files = Directory.GetFiles(source);
            foreach (string filePath in files)
            {
                string fileName = Path.GetFileName(filePath);
                DateTime sourceFileModifiedTime = File.GetLastWriteTime(filePath);
                string destinationFilePath = Path.Combine(destination, getDestinationFileName(fileName));

                bool destinationFileExists = File.Exists(destinationFilePath);
                if(destinationFileExists)
                {
                    DateTime destinationFileModifiedTime = File.GetLastWriteTime(destinationFilePath);
                    if(destinationFileModifiedTime >= sourceFileModifiedTime)
                    {
                        //Console.WriteLine("Skipping file becouse its uptodate {0}", fileName);
                        continue;
                    }
                }

                performTransformationOnFile(filePath, destinationFilePath, ref errors);
            }
        }
    }
}
