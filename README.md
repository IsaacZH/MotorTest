# MotorTest

## ����
- ���Դ�GM6020,RM3508,RM2006���
## ʹ�÷���
- һ��ֻ�ܲ�һ���������������CAN�͵��ID�����ϾͿ����յ���Ϣ�����������Ϣ�͵��״̬��ͬ����<font color=DarkTurquoise>control</font>�ṹ����
- ���еĲ�����<font color=Darkorange>control.c</font>�����ã������õĲ����У�
  - <font color=DarkTurquoise>**pid_type**</font>��Ԥ����6020��3508��2006�ٶȻ���λ�û���PID������<font color=DarkTurquoise>pid_type</font>��ֵ�Ϳ���ѡ�ö�Ӧ��PID
  - <font color=DarkTurquoise>**control_mode**</font>����Ϊ�ٶȻ���λ�û�������<font color=DarkTurquoise>control_mode</font>��ֵ�Ϳ����л�����Ӧ�Ŀ���ģʽ
  - <font color=DarkTurquoise>**target_speed**</font>�����Ŀ���ٶ�
  - <font color=DarkTurquoise>**target_position**</font>�����Ŀ��λ��  
- ����������PID����ֻ�ܸ���<font color=Darkorange>control.c</font>�ж�Ӧ�����飬debugʱҲ��
- Ĭ�Ϲرռ���Ƿ񿪿أ�������뿪�غ������������Ը���<font color=Darkorange>remote.h</font>��<font color=DarkTurquoise>RC_ONLINE</font>�ĺ궨��