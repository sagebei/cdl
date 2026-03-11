def dict_to_latex_table(data, block_size=12):
    """
    Convert a dictionary {size: count} into LaTeX table blocks like:

    Size  & 128 & 129 & ...
    Count & ... & ... & ...
    \\hline
    """

    # Sort by key (size)
    items = sorted(data.items())

    latex_lines = []

    # Process in blocks
    for i in range(0, len(items), block_size):
        block = items[i:i + block_size]

        # Extract sizes and counts
        sizes = [str(k) for k, v in block]
        counts = [str(v) for k, v in block]

        # Build LaTeX rows
        size_row = "Size  & " + " & ".join(sizes) + r"  \\"
        count_row = "Count & " + " & ".join(counts) + r"  \\"

        latex_lines.append(size_row)
        latex_lines.append(count_row)

        # Add separator unless last block
        if i + block_size < len(items):
            latex_lines.append(r"\hline")

    return "\n".join(latex_lines)



