using System.Drawing;
using System.Linq;
using System.Threading;
using System.Windows.Forms;

using System;
using System.Threading.Tasks;
using WampSharp.V2;
using WampSharp.V2.Client;
using WampSharp.V2.Fluent;

using Tobii.Research;
//using Tobii.Research.Addons;

using System.Collections.Concurrent;
using System.Net;

//Install-Package WampSharp.Default -Pre

namespace TobiiRemoteEyeTrackingServer
{
    public partial class Form1 : Form
    {
        private EyeTrackerCollection AvailableTrackers;
        public Form1()
        {
            InitializeComponent();
        }

        private void FindTrackersBtn_Click(object sender, EventArgs e)
        {
            FillInAvailableTrackers();
        }

        private void CalibrateBtn_Click(object sender, EventArgs e)
        {
            try
            {
                //create calibration interface
                NormalizedPoint2D[] pointsToCalibrate = new NormalizedPoint2D[] {
                    new NormalizedPoint2D(0.5f, 0.5f),
                    new NormalizedPoint2D(0.1f, 0.1f),
                    new NormalizedPoint2D(0.1f, 0.9f),
                    new NormalizedPoint2D(0.9f, 0.1f),
                    new NormalizedPoint2D(0.9f, 0.9f),
                };
                Form CalibrationForm = CreateCalibrationForm(pointsToCalibrate);
                //CalibrationForm.Shown += (senderS, eS) =>
                //{
                //    System.Threading.Thread.Sleep(3000);
                //};

                IEyeTracker SelectedTracker = AvailableTrackers.ElementAt(TrackerListComboBox.SelectedIndex);
                CalibrationForm.KeyPress += (keySender, keyEvent) =>
                {
                    if (keyEvent.KeyChar == ' ')
                    {
                        Calibrate(CalibrationForm, SelectedTracker);
                    }
                };



                CalibrationForm.Show();

            }
            catch (Exception exp)
            {
                //do nothing
                Console.Write(exp);
            }
        }

        private Form CreateCalibrationForm(NormalizedPoint2D[] pointsToDraw)
        {
            Form CalibrationForm = new Form();
            CalibrationForm.ControlBox = false;
            CalibrationForm.WindowState = FormWindowState.Maximized;
            //CalibrationForm.Paint += (sender, e) => {
            //    SolidBrush CalBrush = new SolidBrush(Color.Red);
            //    int radius = 25;
            //    int w = CalibrationForm.Width;
            //    int h = CalibrationForm.Height;
            //    foreach (var point in pointsToDraw)
            //    {
            //        e.Graphics.FillEllipse(CalBrush, point.X*w - radius, point.Y*h - radius,
            //          radius + radius, radius + radius);
            //    }
            //};

            return CalibrationForm;
        }
        private Pen CalBrush = new Pen(Color.ForestGreen, 10f);
        private void DrawPointOnForm(Graphics g, int w, int h, NormalizedPoint2D point)
        {
            int radius = 30;
            g.DrawEllipse(CalBrush, point.X * w - radius, point.Y * h - radius,
                      radius + radius, radius + radius);
        }

        private void MovingPoint(Graphics g, int w, int h, NormalizedPoint2D from, NormalizedPoint2D to)
        {
            float fromX = from.X * w;
            float fromY = from.Y * h;
            float toX = to.X * w;
            float toY = to.Y * h;
            float tempX = fromX;
            float tempY = fromY;
            float movingIntervalX = (toX - fromX) / 80;
            float movingIntervalY = (toY - fromY) / 80;
            int radius = 30;
            while ((toX - tempX)*movingIntervalX > 0 || (toY - tempY) * movingIntervalY > 0)
            {
                g.Clear(Color.White);
                g.DrawEllipse(CalBrush, tempX - radius, tempY - radius,
                      radius + radius, radius + radius);
                System.Threading.Thread.Sleep(10);
                tempX += movingIntervalX;
                tempY += movingIntervalY;
            }
            g.Clear(Color.White);
            g.DrawEllipse(CalBrush, toX - radius, toY - radius,
                  radius + radius, radius + radius);
        }

        private void Calibrate(Form CalibrationForm, IEyeTracker eyeTracker)
        {
            // Define the points on screen we should calibrate at.
            // The coordinates are normalized, i.e. (0.0f, 0.0f) is the upper left corner and (1.0f, 1.0f) is the lower right corner.
            var pointsToCalibrate = new NormalizedPoint2D[] {
                new NormalizedPoint2D(0.5f, 0.5f),
                new NormalizedPoint2D(0.1f, 0.1f),
                new NormalizedPoint2D(0.1f, 0.9f),
                new NormalizedPoint2D(0.9f, 0.1f),
                new NormalizedPoint2D(0.9f, 0.9f),
            };

            //eyeTracker.CalibrationChanged += (cSender, cE) =>
            //{
            //    var calibrationValidation = new ScreenBasedCalibrationValidation(eyeTracker);
            //    calibrationValidation.EnterValidationMode();

            //    foreach (var point in pointsToCalibrate)
            //    {
            //        Console.WriteLine("Collecting for point {0}, {1}", point.X, point.Y);

            //        calibrationValidation.StartCollectingData(point);
            //        while (calibrationValidation.State == ScreenBasedCalibrationValidation.ValidationState.CollectingData)
            //        {
            //            System.Threading.Thread.Sleep(25);
            //        }
            //    }

            //    var result = calibrationValidation.Compute();
            //    Console.WriteLine(calibrationValidation);
            //    calibrationValidation.LeaveValidationMode();
                
            //};

            // Create a calibration object.
            var calibration = new ScreenBasedCalibration(eyeTracker);
            // Enter calibration mode.
            calibration.EnterCalibrationMode();

            // Collect data.

            Panel AnimatedPointPanel = new Panel();
            AnimatedPointPanel.Width = CalibrationForm.Width;
            AnimatedPointPanel.Height = CalibrationForm.Height;
            Graphics AnimatedPointGraphics = AnimatedPointPanel.CreateGraphics();
            CalibrationForm.Controls.Add(AnimatedPointPanel);

            int w = CalibrationForm.Width;
            int h = CalibrationForm.Height;
            MovingPoint(AnimatedPointGraphics, w, h, new NormalizedPoint2D(0.5f, 0.1f), new NormalizedPoint2D(0.5f, 0.5f));
            for (int i = 0; i < pointsToCalibrate.Length; i++) 
            {
                var point = pointsToCalibrate[i];
                // Show an image on screen where you want to calibrate.
                DrawPointOnForm(AnimatedPointGraphics, w, h, point);
                Console.WriteLine("Show point on screen at ({0}, {1})", point.X, point.Y);
                // Wait a little for user to focus.
                System.Threading.Thread.Sleep(700);
                // Collect data.
                CalibrationStatus status = calibration.CollectData(point);
                if (status != CalibrationStatus.Success)
                {
                    // Try again if it didn't go well the first time.
                    // Not all eye tracker models will fail at this point, but instead fail on ComputeAndApply.
                    calibration.CollectData(point);
                }
                if(i + 1 < pointsToCalibrate.Length)
                {
                    MovingPoint(AnimatedPointGraphics, w, h, point, pointsToCalibrate[i + 1]);
                }
                
            }
            // Compute and apply the calibration.
            CalibrationResult calibrationResult = calibration.ComputeAndApply();
            Console.WriteLine("Compute and apply returned {0} and collected at {1} points.",
                calibrationResult.Status, calibrationResult.CalibrationPoints.Count);
            // Analyze the data and maybe remove points that weren't good.
            calibration.DiscardData(new NormalizedPoint2D(0.1f, 0.1f));
            // Redo collection at the discarded point.
            Console.WriteLine("Show point on screen at ({0}, {1})", 0.1f, 0.1f);
            calibration.CollectData(new NormalizedPoint2D(0.1f, 0.1f));
            // Compute and apply again.
            calibrationResult = calibration.ComputeAndApply();
            Console.WriteLine("Second compute and apply returned {0} and collected at {1} points.",
                calibrationResult.Status, calibrationResult.CalibrationPoints.Count);
            // See that you're happy with the result.
            // The calibration is done. Leave calibration mode.
            calibration.LeaveCalibrationMode();
            CalibrationForm.Close();
            
        }
        private Thread WAMPThread;
        private bool IsStreamingToWAMP = false;
        private IEyeTracker SelectedTracker = null;
        private IWampSubject WAMPSubject;
        private ConcurrentQueue<GazeDataEventArgs> GazeEventQueue = new ConcurrentQueue<GazeDataEventArgs>();
        private double[] CurrentPupilDiameters = {0, 0 };
        async private void StreamBtn_Click(object sender, EventArgs e)
        {
            
            if (!IsStreamingToWAMP)
            {
                
                //maybe check if the connection before start wamp
                string CrossbarAddress = CrossbarIPTextBox.Text + "/ws";
                string CrossbarRealm = RealmTextbox.Text;

                //This was the old code, might have to do some kind of hybrid depending on the url written.
                /*DefaultWampChannelFactory channelFactory = new DefaultWampChannelFactory();
                IWampChannel channel = channelFactory.CreateJsonChannel("wss://syn.ife.no/crossbarproxy/ws", CrossbarRealm); //CrossbarAddress

                channel.Open().Wait();
                //Task openTask = channel.Open()
                //openTask.Wait(5000);*/

                //if (CrossbarAddress.Contains("wss://")){
                    ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls12;
                //}
                
                WampChannelFactory factory = new WampChannelFactory();

                IWampChannel channel = factory.ConnectToRealm(CrossbarRealm)
                                              .WebSocketTransport(new Uri(CrossbarAddress+"/ws"))
                                              .JsonSerialization()
                                              .Build();

                IWampRealmProxy realmProxy = channel.RealmProxy;

                channel.RealmProxy.Monitor.ConnectionEstablished +=
                    (sender_inner, arg) =>
                    {
                        Console.WriteLine("connected session with ID " + arg.SessionId);

                        dynamic details = arg.WelcomeDetails.OriginalValue.Deserialize<dynamic>();

                        Console.WriteLine("authenticated using method '{0}' and provider '{1}'", details.authmethod,
                                          details.authprovider);

                        Console.WriteLine("authenticated with authid '{0}' and authrole '{1}'", details.authid,
                                          details.authrole);
                    };

                channel.RealmProxy.Monitor.ConnectionBroken +=
                    (sender_inner, arg) =>
                    {
                        dynamic details = arg.Details.OriginalValue.Deserialize<dynamic>();
                        Console.WriteLine("disconnected " + arg.Reason + " " + details.reason + details);
                        Console.WriteLine("disconnected " + arg.Reason);
                    };
              
                await channel.Open().ConfigureAwait(false);

                WAMPSubject = realmProxy.Services.GetSubject("RETDataSample");

                SelectedTracker.GazeDataReceived += HandleGazeData;
                IsStreamingToWAMP = true;
                WAMPThread = new Thread(() => {
                    while(IsStreamingToWAMP)
                    {
                        try
                        {
                            while (GazeEventQueue.Any())
                            {


                                GazeDataEventArgs gazeEvent;
                                GazeEventQueue.TryDequeue(out gazeEvent);
                                float gazeX = 0;
                                float gazeY = 0;
                                if (gazeEvent.LeftEye.GazePoint.Validity == Validity.Valid &&
                                    gazeEvent.RightEye.GazePoint.Validity == Validity.Valid)
                                {
                                    gazeX = (gazeEvent.LeftEye.GazePoint.PositionOnDisplayArea.X + gazeEvent.RightEye.GazePoint.PositionOnDisplayArea.X) / 2;
                                    gazeY = (gazeEvent.LeftEye.GazePoint.PositionOnDisplayArea.Y + gazeEvent.RightEye.GazePoint.PositionOnDisplayArea.Y) / 2;
                                }
                                else if (gazeEvent.LeftEye.GazePoint.Validity == Validity.Valid)
                                {
                                    gazeX = gazeEvent.LeftEye.GazePoint.PositionOnDisplayArea.X;
                                    gazeY = gazeEvent.LeftEye.GazePoint.PositionOnDisplayArea.Y;
                                }
                                else if (gazeEvent.RightEye.GazePoint.Validity == Validity.Valid)
                                {
                                    gazeX = gazeEvent.RightEye.GazePoint.PositionOnDisplayArea.X;
                                    gazeY = gazeEvent.RightEye.GazePoint.PositionOnDisplayArea.Y;
                                }

                                if (gazeEvent.LeftEye.Pupil.PupilDiameter != -1 || gazeEvent.RightEye.Pupil.PupilDiameter != -1)
                                {
                                    CurrentPupilDiameters[0] = Double.IsNaN(gazeEvent.LeftEye.Pupil.PupilDiameter) ? -1 : gazeEvent.LeftEye.Pupil.PupilDiameter;
                                    CurrentPupilDiameters[1] = Double.IsNaN(gazeEvent.RightEye.Pupil.PupilDiameter) ? -1 : gazeEvent.RightEye.Pupil.PupilDiameter;
                                }

                                WampEvent evt = new WampEvent();
                                evt.Arguments = new object[] { SelectedTracker.SerialNumber,
                                new object[] { CurrentPupilDiameters[0],
                                gazeEvent.LeftEye.GazePoint.PositionOnDisplayArea.X,
                                gazeEvent.LeftEye.GazePoint.PositionOnDisplayArea.Y,
                                CurrentPupilDiameters[1],
                                gazeEvent.RightEye.GazePoint.PositionOnDisplayArea.X,
                                gazeEvent.RightEye.GazePoint.PositionOnDisplayArea.Y,
                                gazeEvent.LeftEye.GazeOrigin.PositionInUserCoordinates.X,
                                gazeEvent.LeftEye.GazeOrigin.PositionInUserCoordinates.Y,
                                gazeEvent.LeftEye.GazeOrigin.PositionInUserCoordinates.Z,
                                gazeEvent.RightEye.GazeOrigin.PositionInUserCoordinates.X,
                                gazeEvent.RightEye.GazeOrigin.PositionInUserCoordinates.Y,
                                gazeEvent.RightEye.GazeOrigin.PositionInUserCoordinates.Z,
                                gazeX, gazeY }
                                };
                                WAMPSubject.OnNext(evt);
                            }
                        }
                        catch (Exception exp)
                        {
                            //do nothing, skip
                            Console.Write(exp);
                        }
                        
                    }
                });
                WAMPThread.Start();

                if (StreamBtn.InvokeRequired)
                {
                    StreamBtn.BeginInvoke((MethodInvoker)delegate () { this.StreamBtn.Text = "Stop"; });
                }
                else
                {
                    StreamBtn.Text = "Stop";
                }
            }
            else
            {
                IsStreamingToWAMP = false;
                SelectedTracker.GazeDataReceived -= HandleGazeData;
                if (StreamBtn.InvokeRequired)
                {
                    StreamBtn.BeginInvoke((MethodInvoker)delegate () { this.StreamBtn.Text = "Stream"; });
                }
                else
                {
                    StreamBtn.Text = "Stream";
                }
                WAMPThread.Join();
            }
        }

        private void HandleGazeData(object sender, GazeDataEventArgs gazeEvent)
        {
            try
            {
                if (gazeEvent.LeftEye.Pupil.Validity == Validity.Valid || gazeEvent.RightEye.Pupil.Validity == Validity.Valid)
                {
                    GazeEventQueue.Enqueue(gazeEvent);
                }

            }
            catch (Exception exp)
            {
                Console.Write(exp);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            FillInAvailableTrackers();

            foreach (var screen in Screen.AllScreens)
            {
                DisplayComboBox.Items.Add(screen.DeviceName);
            }

            DisplayComboBox.SelectedIndex = 0;
        }

        private void FillInAvailableTrackers()
        {
            try
            {
                AvailableTrackers = EyeTrackingOperations.FindAllEyeTrackers();
                foreach (IEyeTracker eyeTracker in AvailableTrackers)
                {
                    if (!TrackerListComboBox.Items.Contains(eyeTracker.SerialNumber))
                    {
                        TrackerListComboBox.Items.Add(eyeTracker.SerialNumber);
                    } 
                }
                TrackerListComboBox.SelectedIndex = 0;

                CalibrationPanel.Enabled = (AvailableTrackers.Count > 0);
                WAMPPanel.Enabled = (AvailableTrackers.Count > 0);
                SelectedTracker = AvailableTrackers.ElementAt(TrackerListComboBox.SelectedIndex);
            }
            catch (Exception exp)
            {
                SelectedTracker = null;
                Console.Write(exp);
            }
        }

        private void DisplayComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                SelectedTracker = AvailableTrackers.ElementAt(TrackerListComboBox.SelectedIndex);
                //SelectedTracker.
            }
            catch (Exception exp)
            {
                SelectedTracker = null;
                Console.Write(exp);
            }
        }

        private void TrackerListComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                SelectedTracker = AvailableTrackers.ElementAt(TrackerListComboBox.SelectedIndex);
                foreach (var fre in SelectedTracker.GetAllGazeOutputFrequencies())
                {
                    FrequencyComboBox.Items.Add(fre.ToString());
                }
                FrequencyComboBox.SelectedIndex = 0;

                foreach (var mode in SelectedTracker.GetAllEyeTrackingModes())
                {
                    ModeComboBox.Items.Add(mode.ToString());
                }
                ModeComboBox.SelectedIndex = 0;
            }
            catch (Exception exp)
            {
                SelectedTracker = null;
                Console.Write(exp);
            }
        }

        private void FrequencyComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                foreach (var fre in SelectedTracker.GetAllGazeOutputFrequencies())
                {
                    if (FrequencyComboBox.SelectedValue.Equals(fre.ToString()))
                    {
                        SelectedTracker.SetGazeOutputFrequency(fre);
                        break;
                    }
                }
            }
            catch (Exception exp)
            {
                Console.Write(exp);
            }
        }

        private void ModeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                foreach (var mode in SelectedTracker.GetAllEyeTrackingModes())
                {
                    if (ModeComboBox.SelectedValue.Equals(mode.ToString()))
                    {
                        SelectedTracker.SetEyeTrackingMode(mode);
                        break;
                    }
                }
            }
            catch (Exception exp)
            {
                Console.Write(exp);
            }
        }
    }
}
