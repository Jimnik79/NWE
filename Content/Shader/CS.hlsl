//--------------------------------------------------------------------------------------
// ����: ������������� ������ ���������� ����
// Copyright (�) ������� �. ��������� (Jimnik). ��� ����� ��������.
//--------------------------------------------------------------------------------------

//#define ROW 200	// must be equal to the number of rows/columns specified in code
//struct BufType	// since one stream will work on one row and one column only
//{
//	int a[ROW];                    
//};
//StructuredBuffer<BufType> Buffer0 : register(t0);	// ����� ������ ����

StructuredBuffer<int> Beg : register(t0);		// ����� ������ ����
StructuredBuffer<int> End : register(t1);		// ����� ����� ����
StructuredBuffer<int> Weight : register(t2);	// ����� ���� ����
RWStructuredBuffer<int> Result : register(u0);	// ��������� ���� ����

[numthreads(32, 16, 1)]
void CSMain(uint i : SV_DispatchThreadID)
{
	Result[i] = Weight[i]+4;
}
/*
[numthreads(1, 1, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
	uint z = DTid.z*ROW*ROW + DTid.y*ROW + DTid.x;
	BufferOut[z] = 0;
	
	for(int i=0; i<ROW; i++)
		BufferOut[z] += Buffer0[DTid.y].a[i] * Buffer1[DTid.x].a[i];
}*/

