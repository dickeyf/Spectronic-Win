int roundToInt(double ToRound)
{
	if (((ToRound)-((int)ToRound))>.5)
	{
		return (int)ToRound+1;
	}
	else
		return (int)ToRound;
}