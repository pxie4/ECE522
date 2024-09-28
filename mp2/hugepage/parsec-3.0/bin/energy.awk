BEGIN {
before = 0;
after = 0;
}
{
	if($1=="cpu")
	{
		if(before==0)
			before = $2+$3+$4;
		else
			after = $2+$3+$4;
	}
}
END{
   use = after - before;
   print before,after,use;
}
