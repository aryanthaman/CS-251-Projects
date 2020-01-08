int distance_helper1(NODE* curr, TKey h1)
	{

		int counts = 0;
		while (curr != nullptr)
		{
			if (curr->Key == h1)
			{
				return (counts);
			}
			else if (curr->Key < h1)
			{
				//		prev = curr;
				curr = curr->Right;
			}
			else
			{
				//	prev = curr;
				curr = curr->Left;
			}
			counts++;
		}
		return 0;

	}
	///main helper function

	int distance_helper2(NODE* curr, TKey h2)
	{
		int counts1 = 0;
		while (curr != nullptr)
		{
			if (curr->Key == h2)
			{
				return (counts1);
			}
			else if (curr->Key < h2)
			{
				//prev = curr;
				curr = curr->Right;
			}
			else
			{
				//prev = curr;
				curr = curr->Left;
			}
			counts1++;
		}
		return 0;
	}


	///Main Helper function
	int _distancehelper(NODE* cur, TKey k1, TKey k2)
	{
		assert(Root != nullptr);

		cur = Root;
		if (search(k1) == nullptr || search(k2) == nullptr)
		{
			return -1;
		}

		//if key1 is on the left of the tree and key 2 is on the right simply calculate the distance from the root for both and add them

		else if (k1 > cur->Key && k2 < cur->Key || k1 < cur->Key && k2 > cur->Key)
		{
			return (distance_helper1(Root, k1) + distance_helper2(Root, k2));
		}
		else if (k1 == k2)
		{
			return 0;
		}


		else
		{
			if (k1 == cur->Key || k2 == cur->Key)
			{
				if (k1 == cur->Key)
				{
					return distance_helper1(cur, k2);
				}
				else
				{
					return distance_helper1(cur, k1);
				}
			}

			else if (k1 > cur->Key && k2 > cur->Key)  //case where both on the right
			{
				//do some thing
				search(k1);
				int firstcount = countsgl;
				countsgl = 0;

				search(k2);
				int second = countsgl;
				return (min(firstcount, second));

			}
			else // both on the left side ?
			{
				search(k1);
				int firstcount = countsgl;
				countsgl = 0;

				search(k2);
				int second = countsgl;
				return (min(firstcount, second));

			}

		}
	}