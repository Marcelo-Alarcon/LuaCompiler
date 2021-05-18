function testData(in1, in2) 
	-- test this data
	x = in1
	y = in2
	z = 0

	repeat 
		x = y - 4 * 2
		y = x / 2 - 3
		z = z + 1
	until z < 5

	if (x < y) then
		return 1
	elseif (x > y) then
		return 0
	else
		return nil
	end
end

result1 = testData(6, 4)
result2 = testData(4, 6)
print("TEST")
print(testData(result, result2))
