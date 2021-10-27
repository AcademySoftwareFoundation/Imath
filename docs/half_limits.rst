half Limits
###########

``HALF_DENORM_MIN``
  Smallest positive denormalized half.

``HALF_NRM_MIN``
  Smallest positive normalized half.

``HALF_MIN``
  Smallest positive normalized half.

``HALF_MAX``
  Largest positive half.

``HALF_EPSILON``
  Smallest positive e for which half(1.0 + e) != half(1.0)

``HALF_MANT_DIG``
  Number of digits in mantissa (significand + hidden leading 1)

``HALF_DIG``
  Number of base 10 digits that can be represented without change:

    floor( (``HALF_MANT_DIG`` - 1) * log10(2) ) => 3.01... -> 3

``HALF_DECIMAL_DIG``
  Number of base-10 digits that are necessary to uniquely represent
  all distinct values:

    ceil(``HALF_MANT_DIG`` * log10(2) + 1) => 4.31... -> 5

``HALF_RADIX``
  Base of the exponent.

``HALF_DENORM_MIN_EXP``
  Minimum negative integer such that ``HALF_RADIX`` raised to the
  power of one less than that integer is a normalized half.

``HALF_MAX_EXP``
  Maximum positive integer such that ``HALF_RADIX`` raised to the
  power of one less than that integer is a normalized half.

``HALF_DENORM_MIN_10_EXP``
  Minimum positive integer such that 10 raised to that power is a
  normalized half.

``HALF_MAX_10_EXP``
  Maximum positive integer such that 10 raised to that power is a
  normalized half.
