using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Atomatic_Water_Supply_System
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
            InitializeCore();
        }

        #region Events
        /// <summary>
        /// 串口接收到数据时触发
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void serialPort_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            string data = serialPort.ReadExisting();

            this.Invoke(new Action(() =>
            {
                infoRichTextBox.AppendText(data);
                infoRichTextBox.ScrollToCaret();
            }));
        }

        /// <summary>
        /// 连接端口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void openButton_Click(object sender, EventArgs e)
        {
            bool ret = OpenPort();
            if (ret == true)
            {
                openButton.Enabled = false;
                closeButton.Enabled = true;
                controlGroupBox.Enabled = true;
            }
        }

        /// <summary>
        /// 断开连接
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void closeButton_Click(object sender, EventArgs e)
        {
            bool ret = ClosePort();
            if (ret == true)
            {
                openButton.Enabled = true;
                closeButton.Enabled = false;
                controlGroupBox.Enabled = false;
            }
        }

        /// <summary>
        /// 查找端口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void findButton_Click(object sender, EventArgs e)
        {
            FindPorts();
        }

        /// <summary>
        /// 自动模式
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void autoModeRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (autoModeRadioButton.Checked)
            {
                SendCommand("auto");
            }
        }

        /// <summary>
        /// 手动模式
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void manualModeRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            startButton.Enabled = manualModeRadioButton.Checked;
            if (manualModeRadioButton.Checked)
            {
                SendCommand("manual");
            }
        }

        /// <summary>
        /// 开始供水
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void startButton_Click(object sender, EventArgs e)
        {
            SendCommand("supply");
        }

        /// <summary>
        /// 终止供水
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void stopButton_Click(object sender, EventArgs e)
        {
            SendCommand("stop");
        }

        /// <summary>
        /// 校准时间
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void timeCalibrationButton_Click(object sender, EventArgs e)
        {
            CalibrateTime();
        }

        /// <summary>
        /// 确认发送自动供水时间
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void confirmButton_Click(object sender, EventArgs e)
        {
            SetClock();
        }

        /// <summary>
        /// 右键菜单
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void contextMenuStrip_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            string itemText = e.ClickedItem.ToString();
            switch(itemText)
            {
                case "复制":
                    infoRichTextBox.Copy();
                    break;
                case "清空":
                    infoRichTextBox.Clear();
                    break;
            }
        }
        #endregion

        #region Core code
        /// <summary>
        /// 小工具后台初始化
        /// </summary>
        private void InitializeCore()
        {
            FindPorts();
            baudRateComboBox.SelectedIndex = 1;
            serialPort.Encoding = Encoding.UTF8;
        }

        /// <summary>
        /// 查找计算机可用的端口号，并添加到列表中
        /// </summary>
        private void FindPorts()
        {
            portsComboBox.DataSource = SerialPort.GetPortNames();
            Info(string.Format("查找到{0}个端口", portsComboBox.Items.Count));
        }

        /// <summary>
        /// 配置串口
        /// </summary>
        /// <param name="portName">端口号名称</param>
        /// <param name="baudRate">对应波特率</param>
        /// <returns></returns>
        private bool ConfigurePort(string portName, string baudRate)
        {
            if (String.IsNullOrEmpty(portName))
            {
                Error("端口号为空！");
                return false;
            }

            if (String.IsNullOrEmpty(baudRate))
            {
                Alert("波特率没有选择！");
                return false;
            }

            serialPort.PortName = portName;
            serialPort.BaudRate = int.Parse(baudRate);

            return true;
        }

        /// <summary>
        /// 打开端口
        /// </summary>
        /// <returns>返回为true表示打开端口成功，否则为失败</returns>
        private bool OpenPort()
        {
            bool ret = ConfigurePort(portsComboBox.Text, baudRateComboBox.Text);
            if (ret == false)
            {
                return false;
            }

            try
            {
                serialPort.Open();
                Info("成功打开端口：" + serialPort.PortName);
                return true;
            }
            catch (Exception ex)
            {
                Error(ex.Message);
                return false;
            }
        }

        /// <summary>
        /// 关闭端口
        /// </summary>
        /// <returns>返回为true表示关闭端口成功，否则关闭失败</returns>
        private bool ClosePort()
        {
            try
            {
                serialPort.Close();
                Info("成功关闭端口：" + serialPort.PortName);
                return true;
            }
            catch (Exception ex)
            {
                Error(ex.Message);
                return false;
            }
        }

        /// <summary>
        /// 校准时钟芯片
        /// </summary>
        private void CalibrateTime()
        {
            // 获取当前计算机的时间
            DateTime now = DateTime.Now;

            // 格式化时间
            string timeStr = string.Format(" {0} {1} {2} {3} {4} {5}",
                now.Year.ToString("0000"),
                now.Month.ToString("00"),
                now.Day.ToString("00"),
                now.Hour.ToString("00"),
                now.Minute.ToString("00"),
                now.Second.ToString("00"));

            SendCommand("cali" + timeStr);
        }

        /// <summary>
        /// 自动供水定时时间设置
        /// </summary>
        private void SetClock()
        {
            string timeStr = string.Format(" {0} {1}",
                hourNumericUpDown.Value.ToString("00"),
                minuteNumericUpDown.Value.ToString("00"));
            SendCommand("clock" + timeStr);
        }

        /// <summary>
        /// 发送控制命令给控制器
        /// </summary>
        /// <param name="command"></param>
        /// <returns></returns>
        private void SendCommand(string command)
        {
            if (serialPort.IsOpen == false)
            {
                return;
            }
            serialPort.Write(command);
            Info("发送命令：" + command);
        }
        #endregion

        #region Warnings, errors
        /// <summary>
        /// 警号
        /// </summary>
        /// <param name="message"></param>
        private void Alert(string message)
        {
            MessageBox.Show("警告！", message, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            infoRichTextBox.AppendText("警告：" + message + '\n');
            infoRichTextBox.ScrollToCaret();
        }

        /// <summary>
        /// 错误
        /// </summary>
        /// <param name="message"></param>
        private void Error(string message)
        {
            MessageBox.Show("错误！", message, MessageBoxButtons.OK, MessageBoxIcon.Error);
            infoRichTextBox.AppendText("错误：" + message + '\n');
            infoRichTextBox.ScrollToCaret();
        }

        /// <summary>
        /// 提示
        /// </summary>
        /// <param name="message"></param>
        private void Info(string message)
        {
            infoRichTextBox.AppendText("提示：" + message + '\n');
            infoRichTextBox.ScrollToCaret();
        }

        #endregion
    }
}
